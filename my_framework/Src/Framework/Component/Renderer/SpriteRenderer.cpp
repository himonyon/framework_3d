#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

SpriteRenderer::SpriteRenderer() : SpriteState(eComponentType::SpriteRenderer)
{
	for (int i = 0; i < Sprite::VertexNum; i++) {
		vtx[i].nor = { 0,0,0 };
		vtx[i].col = { 1,1,1,1 };
		vtx[i].pos.z = 0;
	}
}

SpriteRenderer::~SpriteRenderer() {
	SAFE_RELEASE(pVertexBuffer);
}

//コンポーネントの初期化
void SpriteRenderer::SetUpSpriteRenderer(noDel_ptr<Sprite> sprite) {

	if (sprite != nullptr) {
		pRenderSprite = sprite;
		SetDefaultUV();
	}

	for (int i = 0; i < Sprite::VertexNum; i++) {
		vtx[i].nor = { 0,0,0 };
		vtx[i].col = { 1,1,1,1 };
		vtx[i].pos.z = 0;
	}

	vtx[0].pos.x = -0.5f;
	vtx[0].pos.y = 0.5f;
	vtx[1].pos.x = 0.5f;
	vtx[1].pos.y = 0.5f;
	vtx[2].pos.x = -0.5f;
	vtx[2].pos.y = -0.5f;
	vtx[3].pos.x = 0.5f;
	vtx[3].pos.y = -0.5f;

	//バーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(stVertex3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &vtx[0];
	InitData.SysMemPitch = 0;		
	InitData.SysMemSlicePitch = 0;	
	Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
}


void SpriteRenderer::Execute() {
	Render();
}

void SpriteRenderer::Render() {
	if (pRenderSprite == NULL) return;

	//移動・回転・スケーリングの設定
	stVector3 _drawPos = GetPosOnCam();
	stVector3 _drawRot = GetRotOnCam();
	stVector3 _drawScale = GetScaleOnCam();

	Renderer3D::StartRendering();

	// プリミティブの形状を指定
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	stCBuffer3D& _inputCB = Renderer3D::GetInputCB();
	_inputCB.ambient = { 1,1,1,1 };
	_inputCB.diffuse = { color.r, color.g, color.b, color.a };
	_inputCB.specular = { 1,1,1,1 };
	_inputCB.color = { color.r, color.g, color.b, color.a };

	// IA(InputAssemblerStage)に入力レイアウトを設定する
	Direct3D::getDeviceContext()->IASetInputLayout(Renderer3D::GetInputLayout());
	//バーテックスバッファーをセット
	UINT stride = sizeof(stVertex3D);
	UINT offset = 0;
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	Direct3D::getDeviceContext()->IASetIndexBuffer(Renderer3D::GetSpriteIndexBuffer(),DXGI_FORMAT_R32_UINT,0);

	// ワールドマトリクス設定
	XMMATRIX world_matrix;
	XMMATRIX translate = XMMatrixTranslation(_drawPos.x, _drawPos.y, _drawPos.z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(_drawRot.x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(_drawRot.y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(_drawRot.z));
	XMMATRIX scale_mat = XMMatrixScaling(_drawScale.x, _drawScale.y, _drawScale.z);
	world_matrix = scale_mat * rotate_x * rotate_y * rotate_z * translate;

	// ワールドマトリクスをコンスタントバッファに設定
	XMStoreFloat4x4(&_inputCB.world, XMMatrixTranspose(world_matrix));

	// コンスタントバッファ更新
	Direct3D::getDeviceContext()->UpdateSubresource(Renderer3D::GetConstantBuffer(), 0, NULL, &_inputCB, 0, 0);
	// コンテキストにコンスタントバッファを設定
	ID3D11Buffer* _cb = Renderer3D::GetConstantBuffer();
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &_cb);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &_cb);

	//テクスチャーをシェーダーに渡す
	if (pRenderSprite->GetTexture() != NULL)
	{
		ID3D11SamplerState* _samplerState = Renderer3D::GetSampleLinear();
		Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &_samplerState);
		Direct3D::getDeviceContext()->PSSetShaderResources(0, 1, &pRenderSprite->pTextureView);
	}

	//プリミティブをレンダリング
	Direct3D::getDeviceContext()->Draw(4,  0);
}


void SpriteRenderer::SetColor(float r, float g, float b, float a) {
	color.r = r; color.g = g; color.b = b; color.a = a;
}
void SpriteRenderer::SetColor(stColor4 color) {
	this->color.r = color.r; this->color.g = color.g; this->color.b= color.b; this->color.a = color.a;
}
stColor4 SpriteRenderer::GetColor() {
	return { color.r, color.g, color.b, color.a };
}
void SpriteRenderer::SetDefaultUV() {
	if (pRenderSprite == NULL) return; //スプライトがない場合return
	
	vtx[0].tex.x = pRenderSprite->GetVertexState(0).u;
	vtx[0].tex.y = pRenderSprite->GetVertexState(0).v;
	vtx[1].tex.x = pRenderSprite->GetVertexState(1).u;
	vtx[1].tex.y = pRenderSprite->GetVertexState(1).v;
	vtx[2].tex.x = pRenderSprite->GetVertexState(2).u;
	vtx[2].tex.y = pRenderSprite->GetVertexState(2).v;
	vtx[3].tex.x = pRenderSprite->GetVertexState(3).u;
	vtx[3].tex.y = pRenderSprite->GetVertexState(3).v;
}