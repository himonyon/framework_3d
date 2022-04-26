#include "../../../framework.h"
#include "../../../environment.h"

Shader::VertexShader** Shader::vertexShader = NULL;
Shader::PixelShader** Shader::pixelShader = NULL;

bool Shader::InitShader() {
	//���_�V�F�[�_�̍쐬
	vertexShader = new VertexShader * [(unsigned int)eVertexShader::VS_MAX];
	if (vertexShader)
	{
		vertexShader[(unsigned int)eVertexShader::VS_2D] = new VertexShader("Src/Shader/vs_2d.cso");
		vertexShader[(unsigned int)eVertexShader::VS_3D] = new VertexShader("Src/Shader/vs_3d.cso");
	}

	//�s�N�Z���V�F�[�_�̍쐬
	pixelShader = new PixelShader * [(unsigned int)ePixelShader::PS_MAX];
	if (pixelShader)
	{
		pixelShader[(unsigned int)ePixelShader::PS_2D] = new PixelShader("Src/Shader/ps_2d.cso");
		pixelShader[(unsigned int)ePixelShader::PS_3D] = new PixelShader("Src/Shader/ps_3d.cso");
		pixelShader[(unsigned int)ePixelShader::PS_3D_NOTEX] = new PixelShader("Src/Shader/ps_3d_notex.cso");
	}
	return (vertexShader != NULL && pixelShader != NULL);
}

void Shader::DestroyShader() {
	for (int i = 0; i < (unsigned int)eVertexShader::VS_MAX; i++)
	{
		delete vertexShader[i];
		vertexShader[i] = NULL;
	}
	delete[] vertexShader;
	vertexShader = NULL;

	for (int i = 0; i < (unsigned int)ePixelShader::PS_MAX; i++)
	{
		delete pixelShader[i];
		pixelShader[i] = NULL;
	}
	delete[] pixelShader;
	pixelShader = NULL;
}

Shader::ShaderBase::ShaderBase(const char* path)
{
	code = NULL;
	length = 0;

	FILE* fp = NULL;
	if ((fopen_s(&fp, path, "rb")) != 0)
	{
		assert(false);
		return;
	}
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	code = new BYTE[length];
	fread((void*)code, length, 1, fp);

	fclose(fp);
}
Shader::ShaderBase::~ShaderBase()
{
	if (code)delete[] code;
}

//���_�V�F�[�_�[�̃R���X�g���N�g
Shader::VertexShader::VertexShader(const char* path) : ShaderBase(path)
{
	vs = NULL;
	if (!code)return;

	HRESULT hr;
	hr = Direct3D::GetDevice()->CreateVertexShader(
		code,
		length,
		NULL,
		&vs
	);
	if (hr != S_OK)
	{
		assert(false);
	}
}
Shader::VertexShader::~VertexShader()
{
	if (vs)vs->Release();
}

//�s�N�Z���V�F�[�_�[�̃R���X�g���N�^
Shader::PixelShader::PixelShader(const char* path) : ShaderBase(path)
{
	ps = NULL;
	if (!code)return;

	HRESULT hr;
	hr = Direct3D::GetDevice()->CreatePixelShader(
		code,
		length,
		NULL,
		&ps
	);
	if (hr != S_OK)
	{
		assert(false);
	}
}

Shader::PixelShader::~PixelShader()
{
	if (ps)ps->Release();
}

