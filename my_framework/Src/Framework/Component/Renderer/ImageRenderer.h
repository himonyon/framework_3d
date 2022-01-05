#pragma once
/*-----------------------------------------------------------

	SpriteRendererクラス
		スプライトを描画するクラス

-------------------------------------------------------------*/

class ImageRenderer : public Renderer2D {
public:
	ImageRenderer();
	~ImageRenderer(void);

	//頂点情報
	stVertex2D vtx[Sprite::VertexNum];

	//コンポーネント処理
	void Execute() override;

	void SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

	void SetColor(float r, float g, float b, float a) override;
	void SetColor(stColor4 color) override;
	stColor4 GetColor() override;

	void SetDefaultUV(); //スプライトの状態を初期状態に戻す(ColorとUV)

private:
	void Render(void); //描画

	void SetVertexState();
};