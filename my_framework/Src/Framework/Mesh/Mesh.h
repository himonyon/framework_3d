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

class Mesh {
private:
	ID3D11Buffer* pVertexBuffer; //頂点バッファ
	ID3D11Buffer** ppIndexBuffer; //インデックスバッファ
	int materialNum = 0; //モデルが所持するマテリアル数
	stMaterial* pMaterial = 0; //マテリアル

public:
	Mesh(const WCHAR* fileName);
	~Mesh();

	//getter,setter
	ID3D11Buffer* GetPVertexBuffer() const { return pVertexBuffer; }
	ID3D11Buffer** GetPPVertexBuffer() { return &pVertexBuffer; }
	ID3D11Buffer** GetPPIndexBuffer() const { return ppIndexBuffer; }
	stMaterial* GetPMaterials() const { return pMaterial; }
	int GetMaterialNum() const { return materialNum; }

private:
	bool LoadMaterial(LPWSTR fileName, stMaterial** pMaterial); //マテリアル読み込み
};