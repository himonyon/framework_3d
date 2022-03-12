#pragma once
/*-----------------------------------------------------------

	Renderer�N���X
		2D�`��̊��N���X

-------------------------------------------------------------*/

namespace MyFrameWork {
	class Renderer2D : public Component {
	private:
		static D3D11_INPUT_ELEMENT_DESC hInElementDesc_Sprite[];
		static D3D11_INPUT_ELEMENT_DESC hInElementDesc_Model[];

	public:
		//�`��ΏۃX�v���C�g
		noDel_ptr<Sprite> pRenderSprite = NULL;

		//�摜�̕�
		float sizeX = 0;
		float sizeY = 0;

	protected:
		static ID3D11Buffer* pConstantBuffer; //Image�p
		static ID3D11Buffer* pConstantBuffer_sprite;
		static ID3D11Buffer* pIndexBuffer;
		static ID3D11RasterizerState* pRasterizerState;
		static ID3D11SamplerState* pSamplerState;
		static ID3D11BlendState* pBlendState;
		static ID3D11DepthStencilState* pDepthStencilState;
		static ID3D11InputLayout* pInputLayout; //Image�p
		static ID3D11InputLayout* pInputLayout1; //Sprite�p
		static UINT	VertexStrides;
		static UINT	VertexOffsets;

		int renderPriority = 0;

	public:
		//������
		static bool Initialize();
		//�j��
		static void Destroy();

		Renderer2D();
		virtual ~Renderer2D(void);

		//�T�C�Y�ݒ�
		void SetSize(float width, float height);

		//�`�揇�ʂ̐ݒ�
		void SetRenderPriority(int value);
		virtual int GetRenderPriority();

		//�F�̐ݒ�
		virtual void SetColor(float r, float g, float b, float a) {};
		virtual void SetColor(stColor4 color) {};
		virtual stColor4 GetColor() { return { 0,0,0,0 }; };

		//UV�̏�����
		virtual void SetDefaultUV() {};

	private:
		//�`��
		virtual void Render(void) {};
	};
}