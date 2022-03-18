#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

float Renderer3D::WorldWHPos[4] = { 0,0,0,0 };

XMMATRIX Renderer3D::View = {};
XMMATRIX Renderer3D::Proj = {};

ID3D11InputLayout* Renderer3D::pInputLayout = NULL;
ID3D11RasterizerState* Renderer3D::pRasterizerState = NULL;
ID3D11BlendState* Renderer3D::pBlendState = NULL;
ID3D11DepthStencilState* Renderer3D::pDepthStencilState = NULL;
ID3D11Buffer* Renderer3D::pConstantBuffer = NULL;
ID3D11SamplerState* Renderer3D::pSamplerState = NULL;
ID3D11Buffer* Renderer3D::pSpriteIndexBuffer = NULL;
stCBuffer3D  Renderer3D::inputCB = {};

bool Renderer3D::Initialize() {
	//���X�^���C�U�̐ݒ�
	D3D11_RASTERIZER_DESC hRasterizerDesc = {
		D3D11_FILL_SOLID,	//�t�B�����[�h
		D3D11_CULL_NONE,	//�J�����O���[�h
		FALSE,
		0,
		0.0f,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		FALSE
	};

	//���X�^���C�U�̍쐬
	Direct3D::getDevice()->CreateRasterizerState(&hRasterizerDesc, &pRasterizerState);

	//�u�����h�X�e�[�g�̐ݒ�
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

	//�u�����h�X�e�[�g�̍쐬
	Direct3D::getDevice()->CreateBlendState(&BlendDesc, &pBlendState);

	//�[�x�X�e���V���X�e�[�g�̐ݒ�
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//�[�x�X�e���V���X�e�[�g�̍쐬
	Direct3D::getDevice()->CreateDepthStencilState(&dsDesc, &pDepthStencilState);

	//���_���C�A�E�g�쐬
	D3D11_INPUT_ELEMENT_DESC vertex_desc[]{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	if (FAILED(Direct3D::getDevice()->CreateInputLayout(vertex_desc, ARRAYSIZE(vertex_desc),
		Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getCode(), //���_�V�F�[�_�[�̃f�[�^
		Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getLength(), //���_�V�F�[�_�[�̃T�C�Y
		&pInputLayout)))
	{
		return false;
	}

	//�R���X�^���g�o�b�t�@�[�쐬
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

	// �T���v���X�e�[�g�̐ݒ�
	D3D11_SAMPLER_DESC smpDesc;
	memset(&smpDesc, 0, sizeof(smpDesc));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	// �T���v���X�e�[�g�̐���
	Direct3D::getDevice()->CreateSamplerState(&smpDesc, &pSamplerState);

	//Sprite�̃C���f�b�N�X�o�b�t�@�[���쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	int* faceBuffer = new int[6]{ 0,1,2,3,4,5 };
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = faceBuffer;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pSpriteIndexBuffer);

	delete[] faceBuffer;

	return true;
}

void Renderer3D::Destroy() {
	SAFE_RELEASE(pRasterizerState);
	SAFE_RELEASE(pBlendState);
	SAFE_RELEASE(pSamplerState);
	SAFE_RELEASE(pDepthStencilState);
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pSpriteIndexBuffer);
	SAFE_RELEASE(pConstantBuffer);
}

Renderer3D::Renderer3D()
	: Component(eComponentType::Renderer3D)
{
}

Renderer3D::~Renderer3D() {
}

void Renderer3D::StartRendering() {
	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	XMFLOAT3 _EyePt(0.0f, 0.0f, -0.5f); //�J�����i���_�j�ʒu
	XMFLOAT3 _LookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	XMFLOAT3 _UpVec(0.0f, 1.0f, 0.0f);//����ʒu
	XMMATRIX mView = XMMatrixLookAtLH(ToXMVECTOR(_EyePt), ToXMVECTOR(_LookatPt), ToXMVECTOR(_UpVec));
	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	XMMATRIX mProj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);

	View = mView;
	Proj = mProj;
	//��ʒ[�̌v�Z
	stVector3 _screenLT = CalcScreenToXZ(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	stVector3 _screenRB = CalcScreenToXZ(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
	WorldWHPos[0] = _screenLT.x; WorldWHPos[1] = _screenRB.x;
	WorldWHPos[2] = _screenLT.y; WorldWHPos[3] = _screenRB.y;

	// ���C�g�̐ݒ�
	XMVECTOR light = XMVector3Normalize(XMVectorSet(0.0f, 0.5f, -5.0f, 0.0f));
	// �R���X�^���g�o�b�t�@�̐ݒ�
	XMStoreFloat4x4(&inputCB.view, XMMatrixTranspose(mView));
	XMStoreFloat4x4(&inputCB.projection, XMMatrixTranspose(mProj));
	XMStoreFloat4(&inputCB.lightVector, light);
	XMStoreFloat4(&inputCB.cameraPos, ToXMVECTOR(_EyePt));
	inputCB.lightColor = XMFLOAT4(1, 1, 1, 1);

	// �v���~�e�B�u�̌`����w��
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���X�^���C�U�̃Z�b�g
	Direct3D::getDeviceContext()->RSSetState(pRasterizerState);
	//�u�����h�X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetBlendState(pBlendState, NULL, 0xffffffff);
	//�[�x�X�e���V���X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetDepthStencilState(pDepthStencilState, 0);
	Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSamplerState);

	// VerteXShader�APixelShader��ݒ�
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getShader(), nullptr, 0);									// ClassInstance�̐�
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), nullptr, 0);
}

stVector3 Renderer3D::GetPosOnCam() {
	//���݂̍��W�𒸓_���W�ɃZ�b�g
	stVector3 pos;
	pos.x = transform->position.x;
	pos.y = transform->position.y;
	pos.z = transform->position.z;
	//�J�������W�������ăX�N���[�����W��ݒ肷��
	if (Camera::main != nullptr) {
		pos.x -= Camera::main->transform->position.x;
		pos.y -= Camera::main->transform->position.y;
		pos.z -= Camera::main->transform->position.z;
	}
	return stVector3{ pos.x, pos.y, pos.z };
}
stVector3 Renderer3D::GetRotOnCam() {
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
stVector3 Renderer3D::GetScaleOnCam() {
	stVector3 scl;
	scl.x = transform->scale.x;
	scl.y = transform->scale.y;
	scl.z = transform->scale.z;

	if (Camera::main != nullptr) {
		scl.x *= Camera::main->transform->scale.x;
		scl.y *= Camera::main->transform->scale.y;
		scl.z *= Camera::main->transform->scale.z;
	}

	return stVector3{ scl.x, scl.y, scl.z };
}


XMVECTOR Renderer3D::CalcScreenToWorld(
	XMVECTOR& pout,
	int Sx,
	int Sy,
	float fZ,
	int Screen_w,
	int Screen_h) {

	float _x = Camera::main != NULL ? Camera::main->transform->position.x : 0;
	float _y = Camera::main != NULL ? Camera::main->transform->position.y : 0;
	float _z = Camera::main != NULL ? Camera::main->transform->position.z : 0;
	if (-0.2f + _z == 0) _z += -0.2f; //All0���
	XMFLOAT3 tEyePt(0.0f, 0.0f, -0.2f + _z); //�J�����i���_�j�ʒu
	XMFLOAT3 tLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	XMFLOAT3 tUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	XMMATRIX tView = XMMatrixLookAtLH(ToXMVECTOR(tEyePt), ToXMVECTOR(tLookatPt), ToXMVECTOR(tUpVec));
	// �e�s��̋t�s����Z�o
	XMMATRIX InvView, InvPrj, VP, InvViewport;
	InvView = XMMatrixInverse(NULL, tView);
	InvPrj = XMMatrixInverse(NULL, Proj);
	VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] = Screen_w / 2.0f; VP.r[1].m128_f32[1] = -Screen_h / 2.0f;
	VP.r[3].m128_f32[0] = Screen_w / 2.0f; VP.r[3].m128_f32[1] = Screen_h / 2.0f;
	InvViewport = XMMatrixInverse(NULL, VP);

	// �t�ϊ�
	XMMATRIX tmp = InvViewport * InvPrj * InvView;
	XMVECTOR temp = XMVECTOR{ (float)Sx,(float)Sy,fZ, 0 };
	pout = XMVector3TransformCoord(temp, tmp);

	return pout;
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
}

stVector3 Renderer3D::CalcScreenToXZ(
	int Sx,
	int Sy,
	int Screen_w,
	int Screen_h
) {
	XMVECTOR _pout;

	XMVECTOR nearpos;
	XMVECTOR farpos;
	XMVECTOR ray;
	float tem = (float)abs(Camera::main->transform->position.z + 0.2f) / 110.0f;
	CalcScreenToWorld(nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h);
	CalcScreenToWorld(farpos, Sx, Sy, 1.0f, Screen_w, Screen_h);
	ray = farpos - nearpos;
	ray = XMVector3Normalize(ray);

	// XY���ʂƂ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.m128_f32[2] > 0) {
		// XY���ʌ�_
		XMVECTOR temp = XMVECTOR{ 0,0,1,1 };
		XMVECTOR Lray = XMVector3Dot(ray, temp);
		XMVECTOR LP0 = XMVector3Dot((-nearpos), temp);
		_pout = nearpos + (LP0 / Lray) * ray;
		_pout.m128_f32[0] += Camera::main != NULL ? Camera::main->transform->position.x : 0;
		_pout.m128_f32[1] += Camera::main != NULL ? Camera::main->transform->position.y : 0;
	}
	else {
		_pout = farpos;
	}

	//stVector3�ɕϊ�
	stVector3 _newVec;
	_newVec.x = _pout.m128_f32[0];
	_newVec.y = _pout.m128_f32[1];
	_newVec.z = _pout.m128_f32[2];

	return _newVec;
}