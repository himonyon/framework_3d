#pragma once

//3Dオブジェクト頂点構造体
struct stVertex3D {
	XMFLOAT3 pos; //座標
};

struct ConstantBuffer
{
	XMMATRIX WVP;//ワールドから射影までの変換行列
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