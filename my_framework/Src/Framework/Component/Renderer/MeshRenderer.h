#pragma once
/*-----------------------------------------------------------

	MeshRendererコンポーネント
		メッシュを描画する

-------------------------------------------------------------*/

namespace MyFrameWork {
	class MeshRenderer : public Component {
	public:
		//描画対象メッシュ
		noDel_ptr<Mesh> pRenderMesh = NULL;

	private:
		static ID3D11InputLayout* pInputLayout;
		static ID3D11RasterizerState* pRasterizerState;
		static ID3D11BlendState* pBlendState;
		static ID3D11DepthStencilState* pDepthStencilState;
		static ID3D11Buffer* pConstantBuffer;
		static ID3D11SamplerState* pSampleLinear;

		//コンスタントバッファ格納用
		static stCBuffer3D inputCB;

		//カラー
		stColor4 col; 

	public:
		//初期化
		static bool Initialize();
		//破棄
		static void Destroy();

		MeshRenderer();
		~MeshRenderer(void);

		//コンポーネントの初期化
		void SetUpMeshRenderer(noDel_ptr<Mesh> mesh);

		//コンポーネント処理
		void Execute() override;

		//Getter,Setter
		static ID3D11InputLayout* GetInputLayout() { return pInputLayout; }
		static ID3D11RasterizerState* GetRasterizerState() { return pRasterizerState; }
		static ID3D11BlendState* GetBlendState() { return pBlendState; }
		static ID3D11DepthStencilState* GetDepthStencilState() { return pDepthStencilState; }
		static ID3D11Buffer* GetConstantBuffer() { return pConstantBuffer; }
		static ID3D11SamplerState** GetSampleLinear() { return &pSampleLinear; }
		static stCBuffer3D& GetInputCB() { return inputCB; }

	private:
		//描画
		void Render(void);

		//座標、回転、スケールの描画座標(カメラ適用)
		stVector3 GetPosOnCam();
		stVector3 GetRotOnCam();
		stVector3 GetScaleOnCam();
	};
}