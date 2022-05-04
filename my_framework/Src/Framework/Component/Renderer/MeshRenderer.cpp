#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

MeshRenderer::MeshRenderer() : Renderer(eComponentType::MeshRenderer)
{
}

MeshRenderer::~MeshRenderer() {}

//コンポーネントの初期化
void MeshRenderer::SetUpMeshRenderer(noDel_ptr<Mesh> mesh) {
	if (mesh != nullptr) pRenderMesh = mesh;
}

void MeshRenderer::Execute() {
	Render();
}

void MeshRenderer::Render() {

	if (pRenderMesh == NULL) return;
	if (pRenderMesh->GetMeshData() == NULL) return;

	//移動・回転・スケーリングの設定
	stVector3 _drawPos = GetPosOnCam();
	stVector3 _drawRot = GetRotOnCam();
	stVector3 _drawScale = GetScaleOnCam();

	//描画前設定（共通）
	Renderer3D::StartRendering();

	//色読み込み
	stCBuffer3D& _inputCB = Renderer3D::GetInputCB();
	_inputCB.color = { color.r, color.g, color.b, color.a };

	UINT strides = sizeof(stVertex3D);
	UINT offsets = 0;

	// IA(InputAssemblerStage)に入力レイアウトを設定する
	Direct3D::GetDeviceContext()->IASetInputLayout(Renderer3D::GetInputLayout());

	//メッシュデータ取得
	noDel_ptr<stMeshData> _meshData = pRenderMesh->GetMeshData();

	// IAに設定する頂点バッファの指定
	Direct3D::GetDeviceContext()->IASetVertexBuffers(
		0,								// バッファ送信のスロット番号
		1,								// バッファの数
		&_meshData->vertexBuffer,	// 頂点バッファ
		&strides,						// バッファに使用している構造体のサイズ
		&offsets);						// 開始オフセット

	Direct3D::GetDeviceContext()->IASetIndexBuffer(
		_meshData->indexBuffer,
		DXGI_FORMAT_R32_UINT,
		0);

	// ワールドマトリクス設定
	XMMATRIX _world_matrix;
	XMMATRIX _translate = XMMatrixTranslation(_drawPos.x, _drawPos.y, _drawPos.z);
	XMMATRIX _rotateX = XMMatrixRotationX(XMConvertToRadians(_drawRot.x));
	XMMATRIX _rotateY = XMMatrixRotationY(XMConvertToRadians(_drawRot.y));
	XMMATRIX _rotateZ = XMMatrixRotationZ(XMConvertToRadians(_drawRot.z));
	XMMATRIX _scaleMat = XMMatrixScaling(_drawScale.x, _drawScale.y, _drawScale.z);
	_world_matrix = _scaleMat * _rotateX * _rotateY * _rotateZ * _translate;

	// ワールドマトリクスをコンスタントバッファに設定
	XMStoreFloat4x4(&Renderer3D::GetInputCB().world, XMMatrixTranspose(_world_matrix));

	///色設定
	SetMaterialColor(pRenderMesh->GetMaterial());

	// テクスチャ設定
	// Samplerの設定
	ID3D11SamplerState* _sampleState = Renderer3D::GetSampleLinear();
	Direct3D::GetDeviceContext()->PSSetSamplers(0, 1, &_sampleState);	// ID3D11SamplerState

	if (pRenderMesh->GetMaterial()->pTexture != NULL)
	{
		Direct3D::GetDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), nullptr, 0);
		Direct3D::GetDeviceContext()->PSSetShaderResources(0, 1, &pRenderMesh->GetMaterial()->pTexture);
	}
	else
	{
		Direct3D::GetDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D_NOTEX)->getShader(), nullptr, 0);
	}

	// コンスタントバッファ更新
	Direct3D::GetDeviceContext()->UpdateSubresource(Renderer3D::GetConstantBuffer(), 0, NULL, &Renderer3D::GetInputCB(), 0, 0);

	ID3D11Buffer* constant_buffer = Renderer3D::GetConstantBuffer();
	// コンテキストにコンスタントバッファを設定
	Direct3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &constant_buffer);
	Direct3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &constant_buffer);

	stCBuffer3D& cbb = Renderer3D::GetInputCB();

	// 描画
	Direct3D::GetDeviceContext()->DrawIndexed(
		(UINT)_meshData->indices.size(),	// 頂点数
		0,								// オフセット
		0);								// 開始頂点のインデックス
}

void MeshRenderer::SetMaterialColor(noDel_ptr<stMaterial> material)
{
	if (material == NULL) return;
	stCBuffer3D& _inputCB = Renderer3D::GetInputCB();
	_inputCB.diffuse = XMFLOAT4(material->diffuse.r, material->diffuse.g, material->diffuse.b, 1);
	_inputCB.ambient = XMFLOAT4(material->ambient.r, material->ambient.g, material->ambient.b, 1);
}