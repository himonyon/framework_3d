#pragma once
/*-----------------------------------------------------------

	Renderer�N���X
		2D�`��̊��N���X

-------------------------------------------------------------*/

namespace MyFrameWork {
	class Renderer2D {
	private:
		static D3D11_INPUT_ELEMENT_DESC hInElementDesc_Sprite[];

	protected:
		static ID3D11Buffer* pConstantBuffer;
		static ID3D11RasterizerState* pRasterizerState;
		static ID3D11SamplerState* pSamplerState;
		static ID3D11BlendState* pBlendState;
		static ID3D11DepthStencilState* pDepthStencilState;
		static ID3D11InputLayout* pInputLayout;

	public:
		//������
		static bool Initialize();
		//�j��
		static void Destroy();

		Renderer2D();
		~Renderer2D(void);


		//Getter,Setter
		static ID3D11InputLayout* GetInputLayout() { return pInputLayout; }
		static ID3D11RasterizerState* GetRasterizerState() { return pRasterizerState; }
		static ID3D11BlendState* GetBlendState() { return pBlendState; }
		static ID3D11DepthStencilState* GetDepthStencilState() { return pDepthStencilState; }
		static ID3D11Buffer* GetConstantBuffer() { return pConstantBuffer; }
		static ID3D11SamplerState* GetSampleLinear() { return pSamplerState; }
	};
}