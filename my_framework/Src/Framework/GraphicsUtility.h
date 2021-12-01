#pragma once

//float�^���R���\����
struct stVector3 {
public:
	const stVector3 operator -(stVector3 vec) {
		stVector3 temp;
		temp.x = x - vec.x;
		temp.y = y - vec.y;
		temp.z = z - vec.z;
		return temp;
	}
	const stVector3 operator +(stVector3 vec) {
		stVector3 temp;
		temp.x = x + vec.x;
		temp.y = y + vec.y;
		temp.z = z + vec.z;
		return temp;
	}
	const stVector3 operator *(stVector3 vec) {
		stVector3 temp;
		temp.x = x * vec.x;
		temp.y = y * vec.y;
		temp.z = z * vec.z;
		return temp;
	}
	const stVector3 operator /(stVector3 vec) {
		stVector3 temp;
		temp.x = x / vec.x;
		temp.y = y / vec.y;
		temp.z = z / vec.z;
		return temp;
	}
	const stVector3& operator +=(stVector3 vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}
	const stVector3& operator -=(stVector3 vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}
	const stVector3& operator *=(stVector3 vec) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		return *this;
	}
	const stVector3& operator /=(stVector3 vec) {
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		return *this;
	}
	const bool operator !=(stVector3 vec) {
		if (x != vec.x || y != vec.y || z != vec.z) return true;
		return false;
	}

	const bool operator !=(int num) {
		if (x != num || y != num || z != num) return true;
		return false;
	}

	float x;
	float y;
	float z;
};

//�J���[�\����
struct stColor4 {
	float r, g, b, a;
};

//3D�I�u�W�F�N�g���_�\����
struct stVertex3D {
	XMFLOAT3 pos; //���W
	XMFLOAT3 nor; //�@��
	XMFLOAT2 tex; //�e�N�X�`�����W
};


//2D�I�u�W�F�N�g���_�\����
struct stVertex2D
{
	const stVertex2D& operator =(stVertex2D vtx) {
		x = vtx.x; y = vtx.y;
		r = vtx.r; g = vtx.g;
		b = vtx.b; a = vtx.a;
		u = vtx.u; v = vtx.v;
		return *this;
	}

	float x, y;
	float r, g, b, a;
	float u, v;
};

//�}�e���A���\����
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

//���[���h���W�Ԋҗp�R���X�^���g�o�b�t�@
struct ConstantBuffer0
{
	XMMATRIX mW;//���[���h�s��
	XMMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
	XMFLOAT4 lightDir;//���C�g����
	XMFLOAT4 eye;//�J�����ʒu
};

//�}�e���A���p�R���X�^���g�o�b�t�@
struct ConstantBuffer1
{
	XMFLOAT4 ambient;//�A���r�G���g��
	XMFLOAT4 diffuse;//�f�B�t���[�Y�F
	XMFLOAT4 specular;//�X�y�L�����[�i���ʔ��ˁj
};
