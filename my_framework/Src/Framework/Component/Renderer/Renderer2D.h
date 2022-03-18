#pragma once
/*-----------------------------------------------------------

	Rendererクラス
		2D描画の基底クラス

-------------------------------------------------------------*/

namespace MyFrameWork {
	class Renderer2D : public Component {
	private:
		static D3D11_INPUT_ELEMENT_DESC hInElementDesc_Sprite[];

	public:
		//描画対象スプライト
		noDel_ptr<Sprite> pRenderSprite = NULL;

		//画像の幅
		float sizeX = 0;
		float sizeY = 0;

	protected:
		static ID3D11Buffer* pConstantBuffer;
		static ID3D11RasterizerState* pRasterizerState;
		static ID3D11SamplerState* pSamplerState;
		static ID3D11BlendState* pBlendState;
		static ID3D11DepthStencilState* pDepthStencilState;
		static ID3D11InputLayout* pInputLayout;
		static UINT	VertexStrides;
		static UINT	VertexOffsets;

		int renderPriority = 0;

	public:
		//初期化
		static bool Initialize();
		//破棄
		static void Destroy();

		Renderer2D();
		virtual ~Renderer2D(void);

		//サイズ設定
		void SetSize(float width, float height);

		//描画順位の設定
		void SetRenderPriority(int value);
		virtual int GetRenderPriority();

		//色の設定
		virtual void SetColor(float r, float g, float b, float a) {};
		virtual void SetColor(stColor4 color) {};
		virtual stColor4 GetColor() { return { 0,0,0,0 }; };

		//UVの初期化
		virtual void SetDefaultUV() {};

	private:
		//描画
		virtual void Render(void) {};
	};
}