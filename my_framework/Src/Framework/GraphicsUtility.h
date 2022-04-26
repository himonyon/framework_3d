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
	void SetColor(float r, float g, float b, float a) {
		this->r = r; this->g = g; this->b = b; this->a = a;
	}
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

//���b�V���f�[�^
struct stMeshData
{
	stMeshData() {}
	~stMeshData()
	{
		if (vertexBuffer != NULL) vertexBuffer->Release();
		if (indexBuffer != NULL) indexBuffer->Release();
		vertexBuffer = NULL;
		indexBuffer = NULL;
	}
	ID3D11Buffer* vertexBuffer = NULL;	//���_�o�b�t�@(Shader���M�p)
	ID3D11Buffer* indexBuffer = NULL;	//�C���f�b�N�X�o�b�t�@(Shader���M�p)
	std::vector<stVertex3D> vertices;
	std::vector<UINT> indices;
};

struct stMaterial
{
	stMaterial()
	{
		ambient.SetColor(1, 1, 1, 1);
		diffuse.SetColor(1, 1, 1, 1);
		specular.SetColor(1, 1, 1, 1);
		textureKeyWord = "";
		textureName = "";
	}
	~stMaterial()
	{
		if(pTexture != NULL) SAFE_RELEASE(pTexture);
		pTexture = NULL;
	}
	void SetAmbient(float r, float g, float b, float factor)
	{
		ambient.SetColor(r, g, b, factor);
	}
	void SetDiffuse(float r, float g, float b, float factor)
	{
		diffuse.SetColor(r, g, b, factor);
	}
	void SetSpecular(float r, float g, float b, float factor)
	{
		specular.SetColor(r, g, b, factor);
	}

	stColor4 ambient;
	stColor4 diffuse;
	stColor4 specular;

	std::string materialName;
	std::string textureKeyWord;
	std::string textureName;
	ID3D11ShaderResourceView* pTexture = nullptr;
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
