#pragma once

//3Dオブジェクト頂点構造体
struct stVertex3D {
	XMFLOAT3 pos; //座標
	XMFLOAT3 nor; //法線
	XMFLOAT2 tex; //テクスチャ座標
};

struct stMaterial
{
	char name[111];
	XMFLOAT4 ambient;//アンビエント
	XMFLOAT4 diffuse;//ディフューズ
	XMFLOAT4 specular;//スペキュラー
	char textureName[110];//テクスチャーファイル名
	ID3D11ShaderResourceView* pTexture;
	int faceNum;//そのマテリアルであるポリゴン数
	stMaterial()
	{
		ZeroMemory(this, sizeof(stMaterial));
	}
	~stMaterial()
	{
		SAFE_RELEASE(pTexture);
	}
};

struct ConstantBuffer0
{
	XMMATRIX mW;//ワールド行列
	XMMATRIX mWVP;//ワールドから射影までの変換行列
	XMFLOAT4 lightDir;//ライト方向
	XMFLOAT4 eye;//カメラ位置
};

struct ConstantBuffer1
{
	XMFLOAT4 ambient;//アンビエント光
	XMFLOAT4 diffuse;//ディフューズ色
	XMFLOAT4 specular;//スペキュラー（鏡面反射）
};

class Model {
private:
	static ID3D11InputLayout* pVertexLayout;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11BlendState* pBlendState;
	static ID3D11DepthStencilState* pDepthStencilState;
	static ID3D11Buffer* pConstantBuffer0; //行列用
	static ID3D11Buffer* pConstantBuffer1; //マテリアル用
	static ID3D11SamplerState* pSampleLinear;//テクスチャーのサンプラー

	ID3D11Buffer* pVertexBuffer; //頂点バッファ
	ID3D11Buffer** ppIndexBuffer; //インデックスバッファ
	int materialNum = 0; //モデルが所持するマテリアル数
	stMaterial* pMaterial = 0; //マテリアル
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

	bool LoadMaterial(LPWSTR fileName, stMaterial** pMaterial); //マテリアル読み込み
public:
	static bool InitStaticMesh();
	static void DestroyStaticMesh();

	Model(const WCHAR* fileName);
	~Model();

	void Render();
};