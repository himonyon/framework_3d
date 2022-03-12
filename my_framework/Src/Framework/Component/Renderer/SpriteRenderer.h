#pragma once
/*-----------------------------------------------------------

	SpriteRendererコンポーネント
		スプライトを描画する

-------------------------------------------------------------*/

namespace MyFrameWork {
	class SpriteRenderer : public Renderer2D {
	private:
		//頂点バッファ
		ID3D11Buffer* pVertexBuffer = 0;
		ID3D11Buffer* pIndexBuffer = 0;

		static const int indexNum = 6;

		static XMMATRIX View;
		static XMMATRIX Proj;

	public:
		stColor4 color = { 1,1,1,1 };

	public:
		SpriteRenderer();
		~SpriteRenderer(void);

		//画面の上下左右の位置座標
		static float WorldWHPos[4];

		//頂点情報
		stVertex3D vtx[indexNum];

		//コンポーネント処理
		void Execute() override;

		//コンポーネント初期化
		void SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

		//色の設定、取得
		void SetColor(float r, float g, float b, float a) override;
		void SetColor(stColor4 color) override;
		stColor4 GetColor() override;

		//スプライトの状態を初期状態に戻す(ColorとUV)
		void SetDefaultUV() override;

	private:
		//描画
		void Render(void);

		//頂点情報の更新
		void SetVertexState();

		//座標、回転、スケールの描画座標(カメラ適用)
		stVector3 GetPosOnCam();
		stVector3 GetRotOnCam();
		stVector3 GetScaleOnCam();

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