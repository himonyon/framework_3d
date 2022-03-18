#pragma once
/*-----------------------------------------------------------

	SpriteRendererコンポーネント
		スプライトを描画する

-------------------------------------------------------------*/

namespace MyFrameWork {
	class SpriteRenderer : public Renderer3D {
	private:
		//描画対象スプライト
		noDel_ptr<Sprite> pRenderSprite = NULL;

		//頂点バッファ
		ID3D11Buffer* pVertexBuffer = 0;

		static const int indexNum = 6;

	public:
		stColor4 color = { 1,1,1,1 };

	public:
		SpriteRenderer();
		~SpriteRenderer(void);

		//頂点情報
		stVertex3D vtx[indexNum];

		//コンポーネント処理
		void Execute() override;

		//コンポーネント初期化
		void SetUpRenderer(noDel_ptr<Sprite> sprite);

		//色の設定、取得
		void SetColor(float r, float g, float b, float a) override;
		void SetColor(stColor4 color) override;
		stColor4 GetColor() override;

		//スプライトの状態を初期状態に戻す(ColorとUV)
		void SetDefaultUV();

	private:
		//描画
		void Render(void) override;
	};
}