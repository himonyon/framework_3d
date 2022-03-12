/*-----------------------------------------------------------

	�V�F�[�_�[�Ǘ��N���X
		�V�F�[�_�[�t�@�C����ǂݍ���

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {

	class Shader {
	public:
		//�V�F�[�_�[�̎��
		enum class eVertexShader {
			VS_2D,
			VS_3D,
			VS_MAX,
		};
		enum class ePixelShader
		{
			PS_2D,
			PS_3D,
			PS_MAX,
		};

		//�V�F�[�_�[���N���X
		class ShaderBase {
		protected:
			const BYTE* code;
			long length;
		public:
			ShaderBase(const char* path);
			~ShaderBase();

			const BYTE* getCode() { return code; }
			long getLength() { return length; }
		};
		//���_�V�F�[�_�[�N���X
		class VertexShader : public ShaderBase {
		private:
			ID3D11VertexShader* vs;
		public:
			VertexShader(const char* path);
			~VertexShader();

			ID3D11VertexShader* getShader() { return vs; }
		};
		//�s�N�Z���V�F�[�_�[�N���X
		class PixelShader : public ShaderBase {
		private:
			ID3D11PixelShader* ps;
		public:
			PixelShader(const char* path);
			~PixelShader();

			ID3D11PixelShader* getShader() { return ps; }
		};

	private:
		static VertexShader** vertexShader;
		static PixelShader** pixelShader;

	public:
		//������
		static bool InitShader();
		//�j��
		static void DestroyShader();

		//Getter,Setter
		static VertexShader* getVertexShader(eVertexShader vs) { return vertexShader[(unsigned int)vs]; }
		static PixelShader* getPixelShader(ePixelShader ps) { return pixelShader[(unsigned int)ps]; }
	};
}