#include "../../../framework.h"
#include "../../../environment.h"

//頂点要素
D3D11_INPUT_ELEMENT_DESC hInElementDesc_Sprite[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


ID3D11Buffer* GameObject2D::pConstantBuffer = NULL;
ID3D11RasterizerState* GameObject2D::pRasterizerState = 0;
ID3D11SamplerState* GameObject2D::pSamplerState = 0;
ID3D11BlendState* GameObject2D::pBlendState = 0;
ID3D11DepthStencilState* GameObject2D::pDepthStencilState = 0;
ID3D11InputLayout* GameObject2D::pInputLayout = 0;
UINT GameObject2D::VertexStrides = sizeof(stVertex);
UINT GameObject2D::VertexOffsets = 0;

bool GameObject2D::Initialize() {
	//ラスタライザの設定
	D3D11_RASTERIZER_DESC hRasterizerDesc = {
		D3D11_FILL_SOLID,	//フィルモード
		D3D11_CULL_NONE,	//カリングモード
		FALSE,
		0,
		0.0f,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		FALSE
	};

	//ラスタライザの作成
	Direct3D::getDevice()->CreateRasterizerState(&hRasterizerDesc, &pRasterizerState);

	//ブレンドステートの設定
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;

	//ブレンドステートの作成
	Direct3D::getDevice()->CreateBlendState(&BlendDesc, &pBlendState);

	// サンプラステートの設定
	D3D11_SAMPLER_DESC smpDesc;
	memset(&smpDesc, 0, sizeof(smpDesc));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	smpDesc.MinLOD = 0;
	smpDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// サンプラステートの生成
	Direct3D::getDevice()->CreateSamplerState(&smpDesc, &pSamplerState);

	//深度ステンシルステートの設定
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	//深度ステンシルステートの作成
	Direct3D::getDevice()->CreateDepthStencilState(&dsDesc, &pDepthStencilState);

	//頂点レイアウトの作成
	Shader::VertexShader* vs = Shader::getVertexShader(Shader::eVertexShader::VS_2D);
	Direct3D::getDevice()->CreateInputLayout(hInElementDesc_Sprite, ARRAYSIZE(hInElementDesc_Sprite), vs->getCode(), vs->getLength(), &pInputLayout);

	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(float) * 4;
	cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = sizeof(float) * 4;
	cb.Usage = D3D11_USAGE_DEFAULT;


	if (FAILED(Direct3D::getDevice()->CreateBuffer(&cb, NULL, &pConstantBuffer)))
	{
		return FALSE;
	}

	//スクリーン座標をビューポート座標へ変換する係数
	float w = (float)SCREEN_WIDTH;
	float h = (float)SCREEN_HEIGHT;
	float scr[4];
	scr[0] = 2.0f / w;
	scr[1] = -2.0f / h;
	scr[2] = -w * 0.5f;
	scr[3] = -h * 0.5f;
	//定数バッファへ値をセット
	Direct3D::getDeviceContext()->UpdateSubresource(pConstantBuffer, 0, NULL, scr, 0, 0);

	return TRUE;
}

void GameObject2D::Destroy() {
	SAFE_RELEASE(pRasterizerState);
	SAFE_RELEASE(pBlendState);
	SAFE_RELEASE(pSamplerState);
	SAFE_RELEASE(pDepthStencilState);
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pConstantBuffer);
}

GameObject2D::GameObject2D(float x, float y, float width, float height, noDel_ptr<Sprite> sprite, bool isRender, noDel_ptr<GameObject> parent)
	: GameObject(x,y,0,isRender,parent)
{
	renderPriority = 0;
	sortEnable = true;

	sizeX = width;
	sizeY = height;

	for (int i = 0; i < Sprite::VertexNum; i++) {
		color[i].r = 1; color[i].g = 1; color[i].b = 1; color[i].a = 1;
	}

	if (sprite == NULL) return; //スプライトがない場合return
	pRenderSprite = sprite;

	//頂点の設定
	SetVertexState();
}

GameObject2D::~GameObject2D() {
}

void GameObject2D::SetVertexState() {
	float PI = 3.14f;

	//座標
	float vtx_x = 0;
	float vtx_y = 0;

	//各頂点の座標を設定
	vtx_x = position.x + (-sizeX * 0.5f) * scale.x;
	vtx_y = position.y + (-sizeY * 0.5f) * scale.y;
	pRenderSprite->vtx[0].x = (vtx_x - position.x) * cosf(rot.z) - (vtx_y - position.y) * sinf(rot.z) + position.x;
	pRenderSprite->vtx[0].y = (vtx_x - position.x) * sinf(rot.z) + (vtx_y - position.y) * cosf(rot.z) + position.y;

	vtx_x = position.x + (+sizeX * 0.5f) * scale.x;
	vtx_y = position.y + (-sizeY * 0.5f) * scale.y;
	pRenderSprite->vtx[1].x = (vtx_x - position.x) * cosf(rot.z) - (vtx_y - position.y) * sinf(rot.z) + position.x;
	pRenderSprite->vtx[1].y = (vtx_x - position.x) * sinf(rot.z) + (vtx_y - position.y) * cosf(rot.z) + position.y;

	vtx_x = position.x + (-sizeX * 0.5f) * scale.x;
	vtx_y = position.y + (+sizeY * 0.5f) * scale.y;
	pRenderSprite->vtx[2].x = (vtx_x - position.x) * cosf(rot.z) - (vtx_y - position.y) * sinf(rot.z) + position.x;
	pRenderSprite->vtx[2].y = (vtx_x - position.x) * sinf(rot.z) + (vtx_y - position.y) * cosf(rot.z) + position.y;

	vtx_x = position.x + (+sizeX * 0.5f) * scale.x;
	vtx_y = position.y + (+sizeY * 0.5f) * scale.y;
	pRenderSprite->vtx[3].x = (vtx_x - position.x) * cosf(rot.z) - (vtx_y - position.y) * sinf(rot.z) + position.x;
	pRenderSprite->vtx[3].y = (vtx_x - position.x) * sinf(rot.z) + (vtx_y - position.y) * cosf(rot.z) + position.y;

	//カラー
	pRenderSprite->vtx[0].r = color[0].r; pRenderSprite->vtx[0].g = color[0].g;
	pRenderSprite->vtx[0].b = color[0].b; pRenderSprite->vtx[0].a = color[0].a;

	pRenderSprite->vtx[1].r = color[1].r; pRenderSprite->vtx[1].g = color[1].g;
	pRenderSprite->vtx[1].b = color[1].b; pRenderSprite->vtx[1].a = color[1].a;

	pRenderSprite->vtx[2].r = color[2].r; pRenderSprite->vtx[2].g = color[2].g;
	pRenderSprite->vtx[2].b = color[2].b; pRenderSprite->vtx[2].a = color[2].a;

	pRenderSprite->vtx[3].r = color[3].r; pRenderSprite->vtx[3].g = color[3].g;
	pRenderSprite->vtx[3].b = color[3].b; pRenderSprite->vtx[3].a = color[3].a;
}

void GameObject2D::Render() {
	if (pRenderSprite == NULL) return;

	//頂点座標の設定
	SetVertexState();

	//頂点バッファの更新
	Direct3D::getDeviceContext()->UpdateSubresource(pRenderSprite->GetPVertexBuffer(), 0, NULL, pRenderSprite->vtx, 0, 0);

	//ブレンドステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetBlendState(pBlendState, NULL, 0xffffffff);
	//深度ステンシルステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetDepthStencilState(pDepthStencilState, 0);
	//ラスタライザーをコンテキストに設定
	Direct3D::getDeviceContext()->RSSetState(pRasterizerState);
	//頂点インプットレイアウトをセット
	Direct3D::getDeviceContext()->IASetInputLayout(pInputLayout);
	//このコンスタントバッファーを使うシェーダーの登録
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	//バーテックスバッファーをセット
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, pRenderSprite->GetPPVertexBuffer(), &VertexStrides, &VertexOffsets);
	//プリミティブ・トポロジーをセット
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//サンプラーステートをコンテキストに設定
	Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSamplerState);
	//テクスチャーをコンテキストに設定
	Direct3D::setShaderResource(pRenderSprite->GetTextureView());

	//使用するシェーダーの登録	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_2D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_2D)->getShader(), NULL, 0);

	//プリミティブをレンダリング
	Direct3D::getDeviceContext()->Draw(4, 0);
}

//setter/getter
void GameObject2D::SetSize(float width, float height) {
	sizeX = width;
	sizeY = height;
}
void GameObject2D::SetColor(float r, float g, float b, float a) {
	for (int i = 0; i < Sprite::VertexNum; i++) {
		color[i].r = r;
		color[i].g = g;
		color[i].b = b;
		color[i].a = a;
	}
}
void GameObject2D::SetColor(stColor4 color) {
	for (int i = 0; i < Sprite::VertexNum; i++) {
		this->color[i].r = color.r;
		this->color[i].g = color.g;
		this->color[i].b = color.b;
		this->color[i].a = color.a;
	}
}
void GameObject2D::SetRotation(float rot) {
	this->rot.z = rot;
}
void GameObject2D::SetRenderPriority(int value) {
	if (renderPriority != value) sortEnable = true;
	renderPriority = value;
}
int GameObject2D::GetRenderPriority() {
	return renderPriority;
}
void GameObject2D::SetSortEnable(bool flag) {
	sortEnable = flag;
}
bool GameObject2D::isSortEnable() {
	return sortEnable;
}
