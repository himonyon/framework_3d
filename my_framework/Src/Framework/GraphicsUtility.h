#pragma once

//float型を３つもつ構造体
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

//カラー構造体
struct stColor4 {
	float r, g, b, a;
};

//3Dオブジェクト頂点構造体
struct stVertex3D {
	XMFLOAT3 pos; //座標
	XMFLOAT3 nor; //法線
	XMFLOAT2 tex; //テクスチャ座標
};


//2Dオブジェクト頂点構造体
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

//マテリアル構造体
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

//ワールド座標返還用コンスタントバッファ
struct ConstantBuffer0
{
	XMMATRIX mW;//ワールド行列
	XMMATRIX mWVP;//ワールドから射影までの変換行列
	XMFLOAT4 lightDir;//ライト方向
	XMFLOAT4 eye;//カメラ位置
};

//マテリアル用コンスタントバッファ
struct ConstantBuffer1
{
	XMFLOAT4 ambient;//アンビエント光
	XMFLOAT4 diffuse;//ディフューズ色
	XMFLOAT4 specular;//スペキュラー（鏡面反射）
};
