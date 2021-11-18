#pragma once

//3D�I�u�W�F�N�g���_�\����
struct stVertex3D {
	XMFLOAT3 pos; //���W
	XMFLOAT3 nor; //�@��
	XMFLOAT2 tex; //�e�N�X�`�����W
};

struct stMaterial
{
	char name[111];
	XMFLOAT4 ambient;//�A���r�G���g
	XMFLOAT4 diffuse;//�f�B�t���[�Y
	XMFLOAT4 specular;//�X�y�L�����[
	char textureName[110];//�e�N�X�`���[�t�@�C����
	ID3D11ShaderResourceView* pTexture;
	int faceNum;//���̃}�e���A���ł���|���S����
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
	XMMATRIX mW;//���[���h�s��
	XMMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
	XMFLOAT4 lightDir;//���C�g����
	XMFLOAT4 eye;//�J�����ʒu
};

struct ConstantBuffer1
{
	XMFLOAT4 ambient;//�A���r�G���g��
	XMFLOAT4 diffuse;//�f�B�t���[�Y�F
	XMFLOAT4 specular;//�X�y�L�����[�i���ʔ��ˁj
};

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