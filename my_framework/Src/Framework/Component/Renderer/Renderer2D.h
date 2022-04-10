#pragma once
/*-----------------------------------------------------------

	Rendererクラス
		2D描画の基底クラス

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
		//初期化
		static bool Initialize();
		//破棄
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