#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

ImageRenderer::ImageRenderer()
{
	for (int i = 0; i < Sprite::VertexNum; i++) {
		vtx[i].r = 1; vtx[i].g = 1; vtx[i].b = 1; vtx[i].a = 1;
	}
}

ImageRenderer::~ImageRenderer() {
}

//�R���|�[�l���g�̏�����
void ImageRenderer::SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite) {
	this->sizeX = sizeX;
	this->sizeY = sizeY;

	//�X�N���[�����W�n�I�u�W�F�N�g�ɕύX
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

	//���W
	float vtx_x = 0;
	float vtx_y = 0;

	stVector3 pos = transform->position;
	stVector3 scale = transform->scale;
	stVector3 rot = transform->rotation;

	//�e���_�̍��W��ݒ�
	vtx_x = pos.x + (-sizeX * 0.5f) * scale.x;
	vtx_y = pos.y + (-sizeY * 0.5f) * scale.y;
	vtx[0].x = (vtx_x - pos.x) * cosf(rot.z) - (vtx_y - pos.y) * sinf(rot.z) + pos.x;
	vtx[0].y = (vtx_x - pos.x) * sinf(rot.z) + (vtx_y - pos.y) * cosf(rot.z) + pos.y;

	vtx_x = pos.x + (+sizeX * 0.5f) * scale.x;
	vtx_y = pos.y + (-sizeY * 0.5f) * scale.y;
	vtx[1].x = (vtx_x - pos.x) * cosf(rot.z) - (vtx_y - pos.y) * sinf(rot.z) + pos.x;
	vtx[1].y = (vtx_x - pos.x) * sinf(rot.z) + (vtx_y - pos.y) * cosf(rot.z) + pos.y;

	vtx_x = pos.x + (-sizeX * 0.5f) * scale.x;
	vtx_y = pos.y + (+sizeY * 0.5f) * scale.y;
	vtx[2].x = (vtx_x - pos.x) * cosf(rot.z) - (vtx_y - pos.y) * sinf(rot.z) + pos.x;
	vtx[2].y = (vtx_x - pos.x) * sinf(rot.z) + (vtx_y - pos.y) * cosf(rot.z) + pos.y;

	vtx_x = pos.x + (+sizeX * 0.5f) * scale.x;
	vtx_y = pos.y + (+sizeY * 0.5f) * scale.y;
	vtx[3].x = (vtx_x - pos.x) * cosf(rot.z) - (vtx_y - pos.y) * sinf(rot.z) + pos.x;
	vtx[3].y = (vtx_x - pos.x) * sinf(rot.z) + (vtx_y - pos.y) * cosf(rot.z) + pos.y;
}

void ImageRenderer::Render() {
	if (pRenderSprite == NULL) return;

	//���_���W�̐ݒ�
	SetVertexState();

	//���_�o�b�t�@�̍X�V
	Direct3D::getDeviceContext()->UpdateSubresource(pRenderSprite->GetPVertexBuffer(), 0, NULL, vtx, 0, 0);

	//�u�����h�X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetBlendState(pBlendState, NULL, 0xffffffff);
	//�[�x�X�e���V���X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetDepthStencilState(pDepthStencilState, 0);
	//���X�^���C�U�[���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->RSSetState(pRasterizerState);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	Direct3D::getDeviceContext()->IASetInputLayout(pInputLayout1);
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, pRenderSprite->GetPPVertexBuffer(), &VertexStrides, &VertexOffsets);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�T���v���[�X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSamplerState);
	//�e�N�X�`���[���R���e�L�X�g�ɐݒ�
	Direct3D::setShaderResource(pRenderSprite->pTextureView);
	//�g�p����V�F�[�_�[�̓o�^	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_2D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_2D)->getShader(), NULL, 0);

	//�v���~�e�B�u�������_�����O
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
	if (pRenderSprite == NULL) return; //�X�v���C�g���Ȃ��ꍇreturn
	for (int i = 0; i < Sprite::VertexNum; i++) {
		vtx[i].u = pRenderSprite->GetVertexState(i).u;
		vtx[i].v = pRenderSprite->GetVertexState(i).v;
	}
}

int ImageRenderer::GetRenderPriority() {
	int _value = renderPriority;
	_value += isFrontImg ? 100000 : -100000;
	return _value;
}