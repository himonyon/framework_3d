#include "../../../framework.h"
#include "../../../environment.h"
Sprite::Sprite(const WCHAR* texture_file, const char* name, float left_u, float right_u, float top_v, float bottom_v) {
	HRESULT hr;

	this->name = (char*)name;

	//���_�J���[��ݒ�
	for (int i = 0; i < VertexNum; i++)
	{
		vtx[i].r = 1.0f;
		vtx[i].g = 1.0f;
		vtx[i].b = 1.0f;
		vtx[i].a = 1.0f;
	}

	//uv��ݒ�
	vtx[0].u = left_u;	vtx[0].v = top_v;
	vtx[1].u = right_u;	vtx[1].v = top_v;
	vtx[2].u = left_u;	vtx[2].v = bottom_v;
	vtx[3].u = right_u;	vtx[3].v = bottom_v;
	
	//�e�N�X�`���̓ǂݍ���
	hr = DirectX::CreateWICTextureFromFile(Direct3D::getDevice(), texture_file, &pTexture, &pTextureView);
	if (hr != S_OK)
	{
		pTexture = NULL;
		pTextureView = NULL;
	}

	//���_�o�b�t�@�̐ݒ�
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(stVertex2D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);
	//�T�u���\�[�X�f�[�^�̐ݒ�
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vtx;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	if (FAILED(hr))
	{
		assert(false);
	}
}

Sprite::~Sprite() {
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pTextureView);
}