//グローバル
cbuffer global_0:register(b0)
{
	matrix g_mW;//ワールド行列
	matrix g_mWVP; //ワールドから射影までの変換行列
	float4 g_LightDir;  //ライトの方向ベクトル
	float4 g_Eye;//カメラ位置
};

cbuffer global_1:register(b1)
{
	float4 g_Ambient = float4(0, 0, 0, 0);//アンビエント光
	float4 g_Diffuse = float4(1, 0, 0, 0); //拡散反射(色）
	float4 g_Specular = float4(1, 1, 1, 1);//鏡面反射
};


//バーテックスバッファー入力構造体
struct VS_INPUT
{
	float4 position : POSITION0;
	float4 normal : NORMAL0;
	float2 tex : TEXTURE0;
};

//バーテックスバッファー出力構造体
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 tex : TEXTURE0;
	float3 eyeVector : TEXCOORD0;
};

//
//バーテックスシェーダー
//
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	//射影変換（ワールド→ビュー→プロジェクション）
	output.position = mul(input.position, g_mWVP);

	//法線をワールド空間に
	output.normal = mul(input.normal, g_mW);

	//視線ベクトル
	float3 PosWorld = mul(input.position, g_mW);
	output.eyeVector = g_Eye - PosWorld;

	output.normal = normalize(output.normal);

	output.tex = input.tex;

	return output;
}