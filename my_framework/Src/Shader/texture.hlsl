float4 screen : register(b0);

struct VS_INPUT		// 頂点シェーダ
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 tx0 : TEXCOORD0;
};

//================================================================================
// 出力パラメータ
//================================================================================
struct VS_OUTPUT	// 頂点シェーダ
{
	float4 position : SV_POSITION;
	float4 color: COLOR;
	float2 tx0 : TEXCOORD0;
};

//================================================================================
// 頂点シェーダ
//================================================================================
VS_OUTPUT vsMain(VS_INPUT input)
{
	VS_OUTPUT output;

	// 加工なしでそのまま出力
	output.position = float4((input.position.xy + screen.zw) * screen.xy, 0.0, 1.0);
	output.color = input.color;
	output.tx0 = input.tx0;

	return output;
}

// ピクセルシェーダ入力パラメータ
typedef VS_OUTPUT PS_INPUT;		// 置き換え

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//================================================================================
// ピクセルシェーダ
//================================================================================
float4 psMain(PS_INPUT input) : SV_TARGET
{
	float4 color = txDiffuse.Sample(samLinear, input.tx0) * input.color;

	return color;
}