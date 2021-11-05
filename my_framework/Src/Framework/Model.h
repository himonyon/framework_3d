#pragma once

//3D�I�u�W�F�N�g���_�\����
struct stVertex3D {
	XMFLOAT3 pos; //���W
};

struct ConstantBuffer
{
	XMMATRIX WVP;//���[���h����ˉe�܂ł̕ϊ��s��
};

class Model {
private:
	static ID3D11InputLayout* pVertexLayout;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11Buffer* pConstantBuffer;
	int faceNum = 0;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;

	stVertex3D vtx;
public:
	//���W
	float posX = 0;
	float posY = 0;
	float posZ = 0;
	//��]��(���W�A��)
	float rotX = 0;
	float rotY = 0;
	float rotZ = 0;
	//�X�P�[��
	float scaleX = 1;
	float scaleY = 1;
	float scaleZ = 1;

private:
	XMMATRIX GetPosMatrix();
	XMMATRIX GetRotMatrix();
	XMMATRIX GetScaleMatrix();
public:
	static bool InitStaticMesh();
	static void DestroyStaticMesh();

	Model(const WCHAR* fileName);
	~Model();

	void Render();
};