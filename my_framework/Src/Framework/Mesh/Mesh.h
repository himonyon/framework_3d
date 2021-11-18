#pragma once


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