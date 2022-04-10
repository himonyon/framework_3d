#pragma once
/*-----------------------------------------------------------

	ImageRendererコンポーネント
		画像（スクリーン座標系）を描画する

-------------------------------------------------------------*/
namespace MyFrameWork {
	class ImageRenderer : public SpriteState {
	private:
		stVertex2D vtx[Sprite::VertexNum];

	public:
		ImageRenderer();
		~ImageRenderer(void);

		//コンポーネント処理
		void Execute() override;

		//描画対象画像の設定
		void SetUpImageRenderer(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

		//色の設定、取得
		void SetColor(float r, float g, float b, float a) override;
		void SetColor(stColor4 color) override;
		stColor4 GetColor() override;

		void SetDefaultUV() override;

	private:
		//描画
		void Render(void) override;

		//頂点所法の更新
		void SetVertexState();
	};
}