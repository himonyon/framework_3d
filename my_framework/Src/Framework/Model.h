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
private:
	static ID3D11InputLayout* pVertexLayout;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11Buffer* pConstantBuffer;
	int faceNum = 0;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;

	stVertex3D vtx;
public:
	//座標
	float posX = 0;
	float posY = 0;
	float posZ = 0;
	//回転量(ラジアン)
	float rotX = 0;
	float rotY = 0;
	float rotZ = 0;
	//スケール
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