float4 screen : register(b0);

struct VS_INPUT		// 頂点シェーダ
{
	float4 position : POSITION;
	float4 color : COLOR0;
	float2 tx0 : TEXCOORD0;
};

//================================================================================
// 出力パラメータ
//================================================================================
struct VS_OUTPUT	// 頂点シェーダ
{
	float4 position : SV_POSITION;
	float4 color: COLOR0;
	float2 tx0 : TEXCOORD0;
};

//================================================================================
// 頂点シェーダ
//================================================================================
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.position = float4((input.position.xy + screen.zw) * screen.xy, 0.0, 1.0);
	output.color = input.color;
	output.tx0 = input.tx0;

	return output;
}