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
public:
	static bool InitStaticMesh();
	static void DestroyStaticMesh();

	Model(const WCHAR* fileName);
	~Model();

	void Render();
private:
	static ID3D11InputLayout* pVertexLayout;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11Buffer* pConstantBuffer;
	int faceNum = 0;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
};