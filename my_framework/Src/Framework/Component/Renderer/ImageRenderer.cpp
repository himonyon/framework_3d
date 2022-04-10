#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

ImageRenderer::ImageRenderer() : SpriteState(eComponentType::UIRenderer)
{
	for (int i = 0; i < Sprite::VertexNum; i++) {
		vtx[i].r = 1; vtx[i].g = 1; vtx[i].b = 1; vtx[i].a = 1;
	}
}

ImageRenderer::~ImageRenderer() {
}

//コンポーネントの初期化
void ImageRenderer::SetUpImageRenderer(float sizeX, float sizeY, noDel_ptr<Sprite> sprite) {
	size.x = sizeX;
	size.y = sizeY;

	//スクリーン座標系オブジェクトに変更
	gameObject->SetScreenObjType();

	if (sprite != nullptr) {
		pRenderSprite = sprite;
		SetDefaultUV();
	}
}


void ImageRenderer::Execute() {
	Render();
}

void ImageRenderer::SetVertexState() {
	float PI = 3.14f;

	//座標
	float vtx_x = 0;
	float vtx_y = 0;

	stVector3 pos = transform->position;
	stVector3 scale = transform->scale;
	stVector3 rot = transform->rotation;

	//各頂点の座標を設定
	vtx_x = pos.x + (-size.x * 0.5f) * scale.x;
	vtx_y = pos.y + (-size.y * 0.5f) * scale.y;
	vtx[0].x = (vtx_x - pos.x) * cosf(rot.z) - (vtx_y - pos.y) * sinf(rot.z) + pos.x;
	vtx[0].y = (vtx_x - pos.x) * sinf(rot.z) + (vtx_y - pos.y) * cosf(rot.z) + pos.y;

	vtx_x = pos.x + (+size.x * 0.5f) * scale.x;
	vtx_y = pos.y + (-size.y * 0.5f) * scale.y;
	vtx[1].x = (vtx_x - pos.x) * cosf(rot.z) - (vtx_y - pos.y) * sinf(rot.z) + pos.x;
	vtx[1].y = (vtx_x - pos.x) * sinf(rot.z) + (vtx_y - pos.y) * cosf(rot.z) + pos.y;

	vtx_x = pos.x + (-size.x * 0.5f) * scale.x;
	vtx_y = pos.y + (+size.y * 0.5f) * scale.y;
	vtx[2].x = (vtx_x - pos.x) * cosf(rot.z) - (vtx_y - pos.y) * sinf(rot.z) + pos.x;
	vtx[2].y = (vtx_x - pos.x) * sinf(rot.z) + (vtx_y - pos.y) * cosf(rot.z) + pos.y;

	vtx_x = pos.x + (+size.x * 0.5f) * scale.x;
	vtx_y = pos.y + (+size.y * 0.5f) * scale.y;
	vtx[3].x = (vtx_x - pos.x) * cosf(rot.z) - (vtx_y - pos.y) * sinf(rot.z) + pos.x;
	vtx[3].y = (vtx_x - pos.x) * sinf(rot.z) + (vtx_y - pos.y) * cosf(rot.z) + pos.y;
}

void ImageRenderer::Render() {
	if (pRenderSprite == NULL) return;

	//頂点座標の設定
	SetVertexState();

	//頂点バッファの更新
	Direct3D::getDeviceContext()->UpdateSubresource(pRenderSprite->GetPVertexBuffer(), 0, NULL, vtx, 0, 0);

	//ブレンドステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetBlendState(Renderer2D::GetBlendState(), NULL, 0xffffffff);
	//深度ステンシルステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetDepthStencilState(Renderer2D::GetDepthStencilState(), 0);
	//ラスタライザーをコンテキストに設定
	Direct3D::getDeviceContext()->RSSetState(Renderer2D::GetRasterizerState());
	//頂点インプットレイアウトをセット
	Direct3D::getDeviceContext()->IASetInputLayout(Renderer2D::GetInputLayout());
	//このコンスタントバッファーを使うシェーダーの登録
	ID3D11Buffer* _cb = Renderer2D::GetConstantBuffer();
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &_cb);
	//バーテックスバッファーをセット
	UINT strides = sizeof(stVertex2D);
	UINT offsets = 0;
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, pRenderSprite->GetPPVertexBuffer(), &strides, &offsets);
	//プリミティブ・トポロジーをセット
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//サンプラーステートをコンテキストに設定
	ID3D11SamplerState* _sampleState = Renderer2D::GetSampleLinear();
	Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &_sampleState);
	//テクスチャーをコンテキストに設定
	Direct3D::setShaderResource(pRenderSprite->pTextureView);
	//使用するシェーダーの登録	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_2D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_2D)->getShader(), NULL, 0);

	//プリミティブをレンダリング
	Direct3D::getDeviceContext()->Draw(4, 0);
}



void ImageRenderer::SetColor(float r, float g, float b, float a) {
	for (int i = 0; i < Sprite::VertexNum; i++) {
		vtx[i].r = r;
		vtx[i].g = g;
		vtx[i].b = b;
		vtx[i].a = a;
	}
}
void ImageRenderer::SetColor(stColor4 color) {
	for (int i = 0; i < Sprite::VertexNum; i++) {
		this->vtx[i].r = color.r;
		this->vtx[i].g = color.g;
		this->vtx[i].b = color.b;
		this->vtx[i].a = color.a;
	}
}
stColor4 ImageRenderer::GetColor() {
	return stColor4{ vtx[0].r,vtx[0].g, vtx[0].b, vtx[0].a };
}

void ImageRenderer::SetDefaultUV() {
	if (pRenderSprite == NULL) return; //スプライトがない場合return
	for (int i = 0; i < Sprite::VertexNum; i++) {
		vtx[i].u = pRenderSprite->GetVertexState(i).u;
		vtx[i].v = pRenderSprite->GetVertexState(i).v;
	}
}