#pragma once
/*-----------------------------------------------------------

	SpriteRendererコンポーネント
		スプライトを描画する

-------------------------------------------------------------*/

namespace MyFrameWork {
	class SpriteRenderer : public SpriteState {
	private:
		//頂点バッファ
		ID3D11Buffer* pVertexBuffer = 0;

	public:
		SpriteRenderer();
		~SpriteRenderer(void);

		//頂点情報
		stVertex3D vtx[Sprite::VertexNum];

		//コンポーネント処理
		void Execute() override;

		//コンポーネント初期化
		void SetUpSpriteRenderer(noDel_ptr<Sprite> sprite);

		//色の設定、取得
		void SetColor(float r, float g, float b, float a) override;
		void SetColor(stColor4 color) override;
		stColor4 GetColor() override;

		//スプライトの状態を初期状態に戻す(UV)
		void SetDefaultUV() override;

	private:
		//描画
		void Render(void) override;

	};
}