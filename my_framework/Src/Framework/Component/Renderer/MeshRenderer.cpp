#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

ID3D11InputLayout* MeshRenderer::pInputLayout = NULL;
ID3D11RasterizerState* MeshRenderer::pRasterizerState = NULL;
ID3D11BlendState* MeshRenderer::pBlendState = NULL;
ID3D11DepthStencilState* MeshRenderer::pDepthStencilState = NULL;
ID3D11Buffer* MeshRenderer::pConstantBuffer = NULL;
ID3D11SamplerState* MeshRenderer::pSampleLinear = NULL;
stCBuffer3D  MeshRenderer::inputCB = {};

bool MeshRenderer::Initialize() {
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

	//深度ステンシルステートの設定
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//深度ステンシルステートの作成
	Direct3D::getDevice()->CreateDepthStencilState(&dsDesc, &pDepthStencilState);

	//頂点レイアウト作成
	D3D11_INPUT_ELEMENT_DESC vertex_desc[]{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	if (FAILED(Direct3D::getDevice()->CreateInputLayout(vertex_desc, ARRAYSIZE(vertex_desc),
		Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getCode(), //頂点シェーダーのデータ
		Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getLength(), //頂点シェーダーのサイズ
		&pInputLayout)))
	{
		return false;
	}

	//コンスタントバッファー作成
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(stCBuffer3D);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	if (FAILED(Direct3D::getDevice()->CreateBuffer(&buffer_desc, NULL, &pConstantBuffer)))
	{
		return false;
	}

	// サンプラステートの設定
	D3D11_SAMPLER_DESC smpDesc;
	memset(&smpDesc, 0, sizeof(smpDesc));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	// サンプラステートの生成
	Direct3D::getDevice()->CreateSamplerState(&smpDesc, &pSampleLinear);

	return true;
}

void MeshRenderer::Destroy() {
	SAFE_RELEASE(pSampleLinear);
	SAFE_RELEASE(pConstantBuffer);
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pDepthStencilState);
	SAFE_RELEASE(pBlendState);
	SAFE_RELEASE(pRasterizerState);
}

MeshRenderer::MeshRenderer() : Component(eComponentType::Renderer3D)
{
	col = { 1,1,1,1 };
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

	//移動・回転・スケーリングの設定
	stVector3 _drawPos = GetPosOnCam();
	stVector3 _drawRot = GetRotOnCam();
	stVector3 _drawScale = GetScaleOnCam();

	// ビュートランスフォーム（視点座標変換）
	XMFLOAT3 _EyePt(0.0f, 0.0f, -2.5f); //カメラ（視点）位置
	XMFLOAT3 _LookatPt(0.0f, 0.0f, 0.0f);//注視位置
	XMFLOAT3 _UpVec(0.0f, 1.0f, 0.0f);//上方位置
	XMMATRIX mView = XMMatrixLookAtLH(ToXMVECTOR(_EyePt), ToXMVECTOR(_LookatPt), ToXMVECTOR(_UpVec));
	// プロジェクショントランスフォーム（射影変換）
	XMMATRIX mProj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);

	// ライトの設定
	XMVECTOR light = XMVector3Normalize(XMVectorSet(0.0f, 0.5f, -5.0f, 0.0f));

	// コンスタントバッファの設定
	XMStoreFloat4x4(&inputCB.view, XMMatrixTranspose(mView));
	XMStoreFloat4x4(&inputCB.projection, XMMatrixTranspose(mProj));
	XMStoreFloat4(&inputCB.lightVector, light);
	XMStoreFloat4(&inputCB.cameraPos, ToXMVECTOR(_EyePt));
	inputCB.lightColor = XMFLOAT4(1, 1, 1, 1);
	inputCB.color = { col.r, col.g, col.b, col.a };

	// プリミティブの形状を指定
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//ラスタライザのセット
	Direct3D::getDeviceContext()->RSSetState(MeshRenderer::pRasterizerState);
	//ブレンドステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetBlendState(MeshRenderer::pBlendState, NULL, 0xffffffff);
	//深度ステンシルステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetDepthStencilState(MeshRenderer::pDepthStencilState, 0);
	Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &MeshRenderer::pSampleLinear);

	// VerteXShader、PixelShaderを設定
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getShader(), nullptr, 0);									// ClassInstanceの数
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), nullptr, 0);


	pRenderMesh->Render(_drawPos, _drawRot, _drawScale);
}

stVector3 MeshRenderer::GetPosOnCam() {
	//現在の座標を頂点座標にセット
	stVector3 pos;
	pos.x = transform->position.x;
	pos.y = transform->position.y;
	pos.z = transform->position.z;
	//カメラ座標を加えてスクリーン座標を設定する
	if (Camera::main != nullptr) {
		pos.x -= Camera::main->transform->position.x;
		pos.y -= Camera::main->transform->position.y;
		pos.z -= Camera::main->transform->position.z;
	}
	return stVector3{ pos.x, pos.y, pos.z };
}
stVector3 MeshRenderer::GetRotOnCam() {
	stVector3 rot;
	rot.x = transform->rotation.x;
	rot.y = transform->rotation.y;
	rot.z = transform->rotation.z;

	if (Camera::main != nullptr) {
		rot.x -= Camera::main->transform->rotation.x;
		rot.y -= Camera::main->transform->rotation.y;
		rot.z -= Camera::main->transform->rotation.z;
	}
	return stVector3{ rot.x, rot.y, rot.z };
}
stVector3 MeshRenderer::GetScaleOnCam() {
	stVector3 scl;
	scl.x = transform->scale.x;
	scl.y = transform->scale.y;
	scl.z = transform->scale.z;

	return stVector3{ scl.x, scl.y, scl.z };
}