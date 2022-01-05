#include "../../../../framework.h"
#include "../../../../environment.h"



SpriteRenderer::SpriteRenderer()
{
	type = eComponentType::WorldRenderer;
	for (int i = 0; i < 6; i++) {
		vtx[i].pos = { 0,0,0 };
		vtx[i].nor = { 0,0,0 };
		vtx[i].tex = { 0,0 };
	}
}

SpriteRenderer::~SpriteRenderer() {
	SAFE_RELEASE(pVertexBuffer);
}

//�R���|�[�l���g�̏�����
void SpriteRenderer::SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite) {

	this->sizeX = sizeX;
	this->sizeY = sizeY;

	if (sprite != nullptr) {
		pRenderSprite = sprite;
		SetDefaultUV();
	}

	SetVertexState();

	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(stVertex3D) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vtx;
	Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
}


void SpriteRenderer::Execute() {
	Render();
}

void SpriteRenderer::SetVertexState() {
	float PI = 3.14f;

	//�e���_�̍��W��ݒ�
	vtx[0].pos.x = -(sizeX / 2);
	vtx[0].pos.y = -(sizeY / 2);

	vtx[1].pos.x = (sizeX / 2);
	vtx[1].pos.y = -(sizeY / 2);

	vtx[2].pos.x = -(sizeX / 2);
	vtx[2].pos.y = (sizeY / 2);
	
	vtx[3].pos.x = (sizeX / 2);
	vtx[3].pos.y = -(sizeY / 2);

	vtx[4].pos.x = -(sizeX / 2);
	vtx[4].pos.y = (sizeY / 2);

	vtx[5].pos.x = (sizeX / 2);
	vtx[5].pos.y = (sizeY / 2);
}

void SpriteRenderer::Render() {
	if (pRenderSprite == NULL) return;

	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProj;

	mWorld = XMMatrixIdentity();
	mView = XMMatrixIdentity();
	mProj = XMMatrixIdentity();

	// <summary>
	//�ړ��E��]�E�X�P�[�����O
	// </summary>
	//�X�P�[�����O
	XMMATRIX mScale = GetScaleMatrix();
	mWorld = mWorld * mScale;
	//��]
	XMMATRIX mRot = GetRotMatrix();
	mWorld = mWorld * mRot;
	//�ړ�
	XMMATRIX mPos = GetPosMatrix();
	mWorld = mWorld * mPos;

	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	XMFLOAT3 _EyePt(0.0f, 0.0f, -0.2f); //�J�����i���_�j�ʒu
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
	Direct3D::getDeviceContext()->IASetInputLayout(pInputLayout1);
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
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	Direct3D::getDeviceContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//�}�e���A���̕`��
	//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
	D3D11_MAPPED_SUBRESOURCE pData2 = {};
	if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData2)))
	{
		ConstantBuffer1 cb;
		cb.ambient = { 1,1,1,1 };//�A���r�G���g�����V�F�[�_�[�ɓn��
		cb.diffuse = { 1,1,1,1 };//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
		cb.specular = { 1,1,1,1 };//�X�y�L�����[���V�F�[�_�[�ɓn��
		memcpy_s(pData2.pData, pData2.RowPitch, (void*)&cb, sizeof(ConstantBuffer1));
		Direct3D::getDeviceContext()->Unmap(pConstantBuffer1, 0);
	}
	Direct3D::getDeviceContext()->VSSetConstantBuffers(1, 1, &pConstantBuffer1);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(1, 1, &pConstantBuffer1);

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	if (pRenderSprite->GetTexture() != NULL)
	{
		Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSamplerState);
		Direct3D::getDeviceContext()->PSSetShaderResources(0, 1, &pRenderSprite->pTextureView);
	}
	//�v���~�e�B�u�������_�����O
	Direct3D::getDeviceContext()->DrawIndexed(6, 0, 0);
}


void SpriteRenderer::SetColor(float r, float g, float b, float a) {
}
void SpriteRenderer::SetColor(stColor4 color) {
}
stColor4 SpriteRenderer::GetColor() {
	return { 1,1,1,1 };
}
void SpriteRenderer::SetDefaultUV() {
	if (pRenderSprite == NULL) return; //�X�v���C�g���Ȃ��ꍇreturn
	
	vtx[0].tex.x = pRenderSprite->GetVertexState(0).u;
	vtx[0].tex.y = -pRenderSprite->GetVertexState(0).v;
	vtx[1].tex.x = pRenderSprite->GetVertexState(1).u;
	vtx[1].tex.y = -pRenderSprite->GetVertexState(1).v;
	vtx[2].tex.x = pRenderSprite->GetVertexState(2).u;
	vtx[2].tex.y = -pRenderSprite->GetVertexState(2).v;
	vtx[3].tex.x = pRenderSprite->GetVertexState(1).u;
	vtx[3].tex.y = -pRenderSprite->GetVertexState(1).v;
	vtx[4].tex.x = pRenderSprite->GetVertexState(2).u;
	vtx[4].tex.y = -pRenderSprite->GetVertexState(2).v;
	vtx[5].tex.x = pRenderSprite->GetVertexState(3).u;
	vtx[5].tex.y = -pRenderSprite->GetVertexState(3).v;
}

XMMATRIX SpriteRenderer::GetPosMatrix() {
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
	return XMMatrixTranslation(pos.x, pos.y, pos.z);
}
XMMATRIX SpriteRenderer::GetRotMatrix() {
	stVector3 rot;
	rot.x = transform->rotation.x;
	rot.y = transform->rotation.y;
	rot.z = transform->rotation.z;

	if (Camera::main != nullptr) {
		rot.x -= Camera::main->transform->rotation.x;
		rot.y -= Camera::main->transform->rotation.y;
		rot.z -= Camera::main->transform->rotation.z;
	}

	XMMATRIX mPitch, mHeading, mBank;//��]�s��p
	mPitch = XMMatrixRotationX(rot.x);
	mHeading = XMMatrixRotationY(rot.y);
	mBank = XMMatrixRotationZ(rot.z);
	return mPitch * mHeading * mBank;
}

XMMATRIX SpriteRenderer::GetScaleMatrix() {
	return XMMatrixScaling(transform->scale.x, transform->scale.y, transform->scale.z);
}
