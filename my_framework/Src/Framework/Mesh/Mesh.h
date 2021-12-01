#pragma once


class Mesh {
private:
	ID3D11Buffer* pVertexBuffer; //���_�o�b�t�@
	ID3D11Buffer** ppIndexBuffer; //�C���f�b�N�X�o�b�t�@
	int materialNum = 0; //���f������������}�e���A����
	stMaterial* pMaterial = 0; //�}�e���A��

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
	bool LoadMaterial(LPWSTR fileName, stMaterial** pMaterial); //�}�e���A���ǂݍ���
};