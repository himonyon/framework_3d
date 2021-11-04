cbuffer ConstantBuffer {
	matrix WVP; //���[���h����ˉe�܂ł̕ϊ��s��
};

//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct VS_INPUT
{
	float4 position : POSITION0;
};

//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.position = mul(input.position, WVP);

	return output;
}