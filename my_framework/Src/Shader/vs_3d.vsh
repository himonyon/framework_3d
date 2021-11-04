cbuffer ConstantBuffer {
	matrix WVP; //ワールドから射影までの変換行列
};

//バーテックスバッファー出力構造体
struct VS_INPUT
{
	float4 position : POSITION0;
};

//バーテックスバッファー出力構造体
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
};

//
//バーテックスシェーダー
//
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.position = mul(input.position, WVP);

	return output;
}