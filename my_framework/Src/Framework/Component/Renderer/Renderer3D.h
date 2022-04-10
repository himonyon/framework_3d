#pragma once
/*-----------------------------------------------------------

	Rendererクラス
		2D描画の基底クラス

-------------------------------------------------------------*/

namespace MyFrameWork {
	class Renderer3D {
		static XMMATRIX View;
		static XMMATRIX Proj;

	protected:
		static ID3D11Buffer* pConstantBuffer;
		static ID3D11Buffer* pSpriteIndexBuffer;
		static ID3D11RasterizerState* pRasterizerState;
		static ID3D11SamplerState* pSamplerState;
		static ID3D11BlendState* pBlendState;
		static ID3D11DepthStencilState* pDepthStencilState;
		static ID3D11InputLayout* pInputLayout;

		//コンスタントバッファ格納用
		static stCBuffer3D inputCB;

	public:
		//画面の上下左右の位置座標
		static float WorldWHPos[4];

	public:
		//初期化
		static bool Initialize();
		//破棄
		static void Destroy();

		Renderer3D();
		~Renderer3D(void);

		//Getter,Setter
		static ID3D11InputLayout* GetInputLayout() { return pInputLayout; }
		static ID3D11RasterizerState* GetRasterizerState() { return pRasterizerState; }
		static ID3D11BlendState* GetBlendState() { return pBlendState; }
		static ID3D11DepthStencilState* GetDepthStencilState() { return pDepthStencilState; }
		static ID3D11Buffer* GetConstantBuffer() { return pConstantBuffer; }
		static ID3D11Buffer* GetSpriteIndexBuffer() { return pSpriteIndexBuffer; }
		static ID3D11SamplerState* GetSampleLinear() { return pSamplerState; }
		static stCBuffer3D& GetInputCB() { return inputCB; }

		static void StartRendering();

	public:
		static XMVECTOR CalcScreenToWorld(
			XMVECTOR& pout,
			int Sx,
			int Sy,
			float fZ,
			int Screen_w,
			int Screen_h
		);
		// XZ平面とスクリーン座標の交点算出関数
		static stVector3 CalcScreenToXZ(
			int Sx,
			int Sy,
			int Screen_w,
			int Screen_h
		);
	};
}