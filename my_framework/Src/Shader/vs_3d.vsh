//�O���[�o��
cbuffer global_0:register(b0)
{
	matrix g_mW;//���[���h�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_LightDir;  //���C�g�̕����x�N�g��
	float4 g_Eye;//�J�����ʒu
};

cbuffer global_1:register(b1)
{
	float4 g_Ambient = float4(0, 0, 0, 0);//�A���r�G���g��
	float4 g_Diffuse = float4(1, 0, 0, 0); //�g�U����(�F�j
	float4 g_Specular = float4(1, 1, 1, 1);//���ʔ���
};


//�o�[�e�b�N�X�o�b�t�@�[���͍\����
struct VS_INPUT
{
	float4 position : POSITION0;
	float4 normal : NORMAL0;
	float2 tex : TEXTURE0;
};

//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 tex : TEXTURE0;
	float3 eyeVector : TEXCOORD0;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	output.position = mul(input.position, g_mWVP);

	//�@�������[���h��Ԃ�
	output.normal = mul(input.normal, g_mW);

	//�����x�N�g��
	float3 PosWorld = mul(input.position, g_mW);
	output.eyeVector = g_Eye - PosWorld;

	output.normal = normalize(output.normal);

	output.tex = input.tex;

	return output;
}