#include "../../framework.h"
#include "../../environment.h"

//頂点要素
D3D11_INPUT_ELEMENT_DESC hInElementDesc_Sprite[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


ID3D11Buffer* Sprite::pConstantBuffer = NULL;
ID3D11RasterizerState* Sprite::pRasterizerState = 0;
ID3D11SamplerState* Sprite::pSamplerState = 0;
ID3D11BlendState* Sprite::pBlendState = 0;
ID3D11DepthStencilState* Sprite::pDepthStencilState = 0;
ID3D11InputLayout* Sprite::pInputLayout = 0;
UINT Sprite::VertexStrides = sizeof(stVertex2D);
UINT Sprite::VertexOffsets = 0;

bool Sprite::Initialize() {
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

void Sprite::Destroy() {
	SAFE_RELEASE(pRasterizerState);
	SAFE_RELEASE(pBlendState);
	SAFE_RELEASE(pSamplerState);
	SAFE_RELEASE(pDepthStencilState);
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pConstantBuffer);
}

Sprite::Sprite(float x, float y, float width, float height,  const WCHAR* texture_file, Sprite* parent, bool isRender, float left_u, float right_u, float top_v, float bottom_v) {
	HRESULT hr;

	posX = x; 
	posY = y;

	sizeX = width;
	sizeY = height;

	scaleX = 1;
	scaleY = 1;

	pParent = parent;

	//バーテックスバッファー作成
	// //頂点座標を設定
	SetVertexPosition();

	//頂点カラーを設定
	for (int i = 0; i < VertexNum; i++)
	{
		vtx[i].r = 1.0f;
		vtx[i].g = 1.0f;
		vtx[i].b = 1.0f;
		vtx[i].a = 1.0f;
	}

	//uvを設定
	vtx[0].u = left_u;	vtx[0].v = top_v;
	vtx[1].u = right_u;	vtx[1].v = top_v;
	vtx[2].u = left_u;	vtx[2].v = bottom_v;
	vtx[3].u = right_u;	vtx[3].v = bottom_v;
	
	//テクスチャの読み込み
	hr = DirectX::CreateWICTextureFromFile(Direct3D::getDevice(), texture_file, &pTexture, &pTextureView);
	if (hr != S_OK)
	{
		pTexture = NULL;
		pTextureView = NULL;
	}

	//頂点バッファの設定
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(stVertex2D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);
	//サブリソースデータの設定
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vtx;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	if (FAILED(hr))
	{
		assert(false);
	}

	renderPriority = 0;
	sortEnable = false;
	executeEnable = true;
	renderEnable = isRender;

	if (pScene != NULL) {
		pScene->RegisterObject(this);
	}
}

Sprite::~Sprite() {
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pTextureView);
	SAFE_RELEASE(pVertexBuffer);
}

void Sprite::SetVertexPosition() {
	float PI = 3.14f;

	float vtx_x = 0;
	float vtx_y = 0;

	//各頂点の座標を設定
	vtx_x = posX + (-sizeX * 0.5f) * scaleX;
	vtx_y = posY + (-sizeY * 0.5f) * scaleY;
	vtx[0].x = (vtx_x - posX) * cosf(rot) - (vtx_y - posY) * sinf(rot) + posX;
	vtx[0].y = (vtx_x - posX) * sinf(rot) + (vtx_y - posY) * cosf(rot) + posY;

	vtx_x = posX + (+sizeX * 0.5f) * scaleX;
	vtx_y = posY + (-sizeY * 0.5f) * scaleY;
	vtx[1].x = (vtx_x - posX) * cosf(rot) - (vtx_y - posY) * sinf(rot) + posX;
	vtx[1].y = (vtx_x - posX) * sinf(rot) + (vtx_y - posY) * cosf(rot) + posY;

	vtx_x = posX + (-sizeX * 0.5f) * scaleX;
	vtx_y = posY + (+sizeY * 0.5f) * scaleY;
	vtx[2].x = (vtx_x - posX) * cosf(rot) - (vtx_y - posY) * sinf(rot) + posX;
	vtx[2].y = (vtx_x - posX) * sinf(rot) + (vtx_y - posY) * cosf(rot) + posY;

	vtx_x = posX + (+sizeX * 0.5f) * scaleX;
	vtx_y = posY + (+sizeY * 0.5f) * scaleY;
	vtx[3].x = (vtx_x - posX) * cosf(rot) - (vtx_y - posY) * sinf(rot) + posX;
	vtx[3].y = (vtx_x - posX) * sinf(rot) + (vtx_y - posY) * cosf(rot) + posY;
}

void Sprite::Execute() {
}

void Sprite::Render() {
	
	//頂点座標の設定
	SetVertexPosition();

	//頂点バッファの更新
	Direct3D::getDeviceContext()->UpdateSubresource(pVertexBuffer, 0, NULL, vtx, 0, 0);

	//頂点インプットレイアウトをセット
	Direct3D::getDeviceContext()->IASetInputLayout(pInputLayout);
	//このコンスタントバッファーを使うシェーダーの登録
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	//バーテックスバッファーをセット
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &VertexStrides, &VertexOffsets);
	//プリミティブ・トポロジーをセット
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//サンプラーステートをコンテキストに設定
	Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSamplerState);
	//テクスチャーをコンテキストに設定
	Direct3D::setShaderResource(pTextureView);

	//使用するシェーダーの登録	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_2D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_2D)->getShader(), NULL, 0);
	
	//プリミティブをレンダリング
	Direct3D::getDeviceContext()->Draw(4, 0);
}

//setter/getter
void Sprite::SetPosition(float x, float y) {
	posX = x;
	posY = y;
}
void Sprite::SetColor(float r, float g, float b, float a) {
	for (int i = 0; i < VertexNum; i++) {
		vtx[i].r = r;
		vtx[i].g = g;
		vtx[i].b = b;
		vtx[i].a = a;
	}
}
void Sprite::SetScale(float x, float y) {
	scaleX = x;
	scaleY = y;
}

void Sprite::SetRenderPriority(int value) {
	if (renderPriority != value) sortEnable = true;
	renderPriority = value;
}

int Sprite::GetRenderPriority() {
	return renderPriority;
}

void Sprite::SetSortEnable(bool flag) {
	sortEnable = flag;
}

bool Sprite::isSortEnable() {
	return sortEnable;
}

void Sprite::SetExecuteEnable(bool flag) {
	executeEnable = flag;
}

void Sprite::SetRenderEnable(bool flag) {
	renderEnable = flag;
}

bool Sprite::isExecuteEnable() {
	return executeEnable;
}

bool Sprite::isRenderEnable() {
	return renderEnable;
}

void Sprite::TextureCopy(Sprite* copy_to, Sprite* copy_from) {
	memcpy(&copy_to->pTexture,  &copy_from->pTexture, sizeof(ID3D11Resource));
	memcpy(&copy_to->pTextureView, &copy_from->pTextureView, sizeof(ID3D11ShaderResourceView));
	memcpy(&copy_to->pVertexBuffer, &copy_from->pVertexBuffer, sizeof(ID3D11Buffer));
}