#include "../../../framework.h"
#include "../../../environment.h"

_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT3& vec)
{
	return DirectX::XMLoadFloat3(&vec);
}

//���_�C���v�b�g���C�A�E�g���`	
D3D11_INPUT_ELEMENT_DESC hInElementDesc_Model[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ID3D11InputLayout* GameObject3D::pVertexLayout = NULL;
ID3D11RasterizerState* GameObject3D::pRasterizerState = NULL;
ID3D11BlendState* GameObject3D::pBlendState = NULL;
ID3D11DepthStencilState* GameObject3D::pDepthStencilState = NULL;
ID3D11Buffer* GameObject3D::pConstantBuffer0 = NULL;
ID3D11Buffer* GameObject3D::pConstantBuffer1 = NULL;
ID3D11SamplerState* GameObject3D::pSampleLinear = NULL;

bool GameObject3D::Initialize() {
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

	//���_�C���v�b�g���C�A�E�g���쐬
	Shader::VertexShader* vs = Shader::getVertexShader(Shader::eVertexShader::VS_3D);
	UINT numElements = sizeof(hInElementDesc_Model) / sizeof(hInElementDesc_Model[0]);
	if (FAILED(Direct3D::getDevice()->CreateInputLayout(hInElementDesc_Model, numElements, vs->getCode(), vs->getLength(), &pVertexLayout)))
	{
		return FALSE;
	}

	//�R���X�^���g�o�b�t�@�[�쐬(�ϊ��s��p)
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(Direct3D::getDevice()->CreateBuffer(&cb, NULL, &pConstantBuffer0)))
	{
		return false;
	}
	//�R���X�^���g�o�b�t�@�[�쐬(�}�e���A���p)
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(Direct3D::getDevice()->CreateBuffer(&cb, NULL, &pConstantBuffer1)))
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
	Direct3D::getDevice()->CreateSamplerState(&smpDesc, &pSampleLinear);

	return true;
}

void GameObject3D::Destroy() {
	SAFE_RELEASE(pSampleLinear);
	SAFE_RELEASE(pConstantBuffer0);
	SAFE_RELEASE(pConstantBuffer1);
	SAFE_RELEASE(pVertexLayout);
	SAFE_RELEASE(pDepthStencilState);
	SAFE_RELEASE(pBlendState);
	SAFE_RELEASE(pRasterizerState);
}

GameObject3D::GameObject3D(float x, float y, float z, noDel_ptr<Mesh> mesh, bool isRender, noDel_ptr<GameObject> parent)
	: GameObject(x, y, z, isRender, parent)
{
	if (mesh == nullptr) return;
	pRenderMesh = mesh;
}

GameObject3D::~GameObject3D(){}

void GameObject3D::Render() {
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProj;

	mWorld = XMMatrixIdentity();
	mView = XMMatrixIdentity();
	mProj = XMMatrixIdentity();

	// <summary>
	//�ړ��E��]�E�X�P�[�����O�̐ݒ�
	// </summary>
	//�X�P�[�����O
	XMMATRIX mScale = GetScaleMatrix();
	mWorld = mWorld * mScale;//���[���h�s��֔��f
	//��]
	XMMATRIX mRot = GetRotMatrix();
	mWorld = mWorld * mRot;
	//�ړ�
	XMMATRIX mPos = GetPosMatrix();
	mWorld = mWorld * mPos;

	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	XMFLOAT3 _EyePt(0.0f, 0.0f, -0.5f); //�J�����i���_�j�ʒu
	XMFLOAT3 _LookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	XMFLOAT3 _UpVec(0.0f, 1.0f, 0.0f);//����ʒu
	mView = XMMatrixLookAtLH(ToXMVECTOR(_EyePt), ToXMVECTOR(_LookatPt), ToXMVECTOR(_UpVec));
	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	mProj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);


	//�g�p����V�F�[�_�[�̓o�^	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), NULL, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBuffer0 cb;
		//���[���h�s���n��
		cb.mW = mWorld;
		cb.mW = XMMatrixTranspose(cb.mW);
		//���[���h�A�J�����A�ˉe�s���n��
		XMMATRIX m = mWorld * mView * mProj;
		cb.mWVP = m;
		cb.mWVP = XMMatrixTranspose(cb.mWVP);
		//���C�g�̕�����n��
		XMFLOAT3 vLightDir(0, 0, 0);
		cb.lightDir = XMFLOAT4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);
		//���_�ʒu��n��
		cb.eye = XMFLOAT4(_EyePt.x, _EyePt.y, _EyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBuffer0));
		Direct3D::getDeviceContext()->Unmap(pConstantBuffer0, 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer0);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &pConstantBuffer0);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	Direct3D::getDeviceContext()->IASetInputLayout(pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//���X�^���C�U�̃Z�b�g
	Direct3D::getDeviceContext()->RSSetState(pRasterizerState);
	//�u�����h�X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetBlendState(pBlendState, NULL, 0xffffffff);
	//�[�x�X�e���V���X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetDepthStencilState(pDepthStencilState, 0);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(stVertex3D);
	UINT offset = 0;
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, pRenderMesh->GetPPVertexBuffer(), &stride, &offset);
	//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
	for (int i = 0; i < pRenderMesh->GetMaterialNum(); i++)
	{
		//�g�p����Ă��Ȃ��}�e���A���΍�
		if (pRenderMesh->GetPMaterials()[i].faceNum == 0)
		{
			continue;
		}
		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		stride = sizeof(int);
		offset = 0;
		Direct3D::getDeviceContext()->IASetIndexBuffer(pRenderMesh->GetPPIndexBuffer()[i], DXGI_FORMAT_R32_UINT, 0);
		//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			ConstantBuffer1 cb;
			cb.ambient = pRenderMesh->GetPMaterials()[i].ambient;//�A���r�G���g�����V�F�[�_�[�ɓn��
			cb.diffuse = pRenderMesh->GetPMaterials()[i].diffuse;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
			cb.specular = pRenderMesh->GetPMaterials()[i].specular;//�X�y�L�����[���V�F�[�_�[�ɓn��
			memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBuffer1));
			Direct3D::getDeviceContext()->Unmap(pConstantBuffer1, 0);
		}
		Direct3D::getDeviceContext()->VSSetConstantBuffers(1, 1, &pConstantBuffer1);
		Direct3D::getDeviceContext()->PSSetConstantBuffers(1, 1, &pConstantBuffer1);

		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		if (pRenderMesh->GetPMaterials()[i].textureName[0] != NULL)
		{
			Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSampleLinear);
			Direct3D::getDeviceContext()->PSSetShaderResources(0, 1, &pRenderMesh->GetPMaterials()[i].pTexture);
		}
		//�v���~�e�B�u�������_�����O
		Direct3D::getDeviceContext()->DrawIndexed(pRenderMesh->GetPMaterials()[i].faceNum * 3, 0, 0);
	}
}

XMMATRIX GameObject3D::GetPosMatrix() {
	//���݂̍��W�𒸓_���W�ɃZ�b�g
	return XMMatrixTranslation(position.x, -position.y, position.z);
}
XMMATRIX GameObject3D::GetRotMatrix() {
	XMMATRIX mPitch, mHeading, mBank;//��]�s��p
	mPitch = XMMatrixRotationX(rot.x);
	mHeading = XMMatrixRotationY(rot.y);
	mBank = XMMatrixRotationZ(rot.z);
	return mPitch * mHeading * mBank;
}
XMMATRIX GameObject3D::GetScaleMatrix() {
	return XMMatrixScaling(scale.x, scale.y, scale.z);
}