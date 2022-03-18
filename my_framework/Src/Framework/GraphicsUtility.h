#pragma once

//float�^��2���\����
struct stVector2 {
public:
	const stVector2 operator -(stVector2 vec) {
		stVector2 temp;
		temp.x = x - vec.x;
		temp.y = y - vec.y;
		return temp;
	}
	const stVector2 operator +(stVector2 vec) {
		stVector2 temp;
		temp.x = x + vec.x;
		temp.y = y + vec.y;
		return temp;
	}
	const stVector2 operator *(stVector2 vec) {
		stVector2 temp;
		temp.x = x * vec.x;
		temp.y = y * vec.y;
		return temp;
	}
	const stVector2 operator /(stVector2 vec) {
		stVector2 temp;
		temp.x = x / vec.x;
		temp.y = y / vec.y;
		return temp;
	}
	const stVector2& operator +=(stVector2 vec) {
		x += vec.x;
		y += vec.y;
		return *this;
	}
	const stVector2& operator -=(stVector2 vec) {
		x -= vec.x;
		y -= vec.y;
		return *this;
	}
	const stVector2& operator *=(stVector2 vec) {
		x *= vec.x;
		y *= vec.y;
		return *this;
	}
	const stVector2& operator /=(stVector2 vec) {
		x /= vec.x;
		y /= vec.y;
		return *this;
	}
	const bool operator !=(stVector2 vec) {
		if (x != vec.x || y != vec.y ) return true;
		return false;
	}

	const bool operator !=(int num) {
		if (x != num || y != num) return true;
		return false;
	}

	float x;
	float y;
};
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
	stVector3 pos; //���W
	stVector3 nor; //�@��
	stColor4 col; //�F
	stVector2 tex; //�e�N�X�`�����W
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

struct stMaterial
{
	stMaterial()
	{
		for (int i = 0; i < 4; i++)
		{
			Ambient[i] = 1.0f;
			Diffuse[i] = 1.0f;
			Specular[i] = 1.0f;
		}
		TextureKeyWord = "";
		TextureName = "";
	}

	void SetAmbient(float r, float g, float b, float factor)
	{
		Ambient[0] = r;
		Ambient[1] = g;
		Ambient[2] = b;
		Ambient[3] = factor;
	}

	void SetDiffuse(float r, float g, float b, float factor)
	{
		Diffuse[0] = r;
		Diffuse[1] = g;
		Diffuse[2] = b;
		Diffuse[3] = factor;
	}

	void SetSpecular(float r, float g, float b, float factor)
	{
		Specular[0] = r;
		Specular[1] = g;
		Specular[2] = b;
		Specular[3] = factor;
	}

	float Ambient[4];
	float Diffuse[4];
	float Specular[4];
	std::string TextureKeyWord;
	std::string TextureName;
};

//Sprite�̃R���X�^���g�o�b�t�@
struct stCBufferSprite
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4 cameraPos;
	stColor4 color;
};

//3D�̃R���X�^���g�o�b�t�@
struct stCBuffer3D
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4 cameraPos;
	XMFLOAT4 lightVector;
	XMFLOAT4 lightColor;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	stColor4 color;
};
