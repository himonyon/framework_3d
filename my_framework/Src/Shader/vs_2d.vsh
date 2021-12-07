float4 screen : register(b0);

struct VS_INPUT		// ���_�V�F�[�_
{
	float4 position : POSITION;
	float4 color : COLOR0;
	float2 tx0 : TEXCOORD0;
};

//================================================================================
// �o�̓p�����[�^
//================================================================================
struct VS_OUTPUT	// ���_�V�F�[�_
{
	float4 position : SV_POSITION;
	float4 color: COLOR0;
	float2 tx0 : TEXCOORD0;
};

//================================================================================
// ���_�V�F�[�_
//================================================================================
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.position = float4((input.position.xy + screen.zw) * screen.xy, 0.0, 1.0);
	output.color = input.color;
	output.tx0 = input.tx0;

	return output;
}