#pragma once
/*-----------------------------------------------------------

	ImageRendererコンポーネント
		画像（スクリーン座標系）を描画する

-------------------------------------------------------------*/
namespace MyFrameWork {
	class ImageRenderer : public Renderer2D {
	public:
		//ワールド座標オブジェクトの手前か奥どちらに描画するか
		bool isFrontImg = true;

	public:
		ImageRenderer();
		~ImageRenderer(void);

		//頂点情報
		stVertex2D vtx[Sprite::VertexNum];

		//コンポーネント処理
		void Execute() override;

		//描画対象画像の設定
		void SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

		//色の設定、取得
		void SetColor(float r, float g, float b, float a) override;
		void SetColor(stColor4 color) override;
		stColor4 GetColor() override;

		//描画純の設定：FrontObjなら+10000した値、BackObjなら-10000した値を返す
		int GetRenderPriority() override;
		//スプライトの状態を初期状態に戻す(ColorとUV)
		void SetDefaultUV() override;

	private:
		//描画
		void Render(void);
		//頂点所法の更新
		void SetVertexState();
	};
}