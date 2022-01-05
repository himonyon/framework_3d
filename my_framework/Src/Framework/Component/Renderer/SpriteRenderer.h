#pragma once
/*-----------------------------------------------------------

	SpriteRendererクラス
		スプライトを描画するクラス

-------------------------------------------------------------*/

class SpriteRenderer : public Renderer2D {
private:
	//頂点バッファ
	ID3D11Buffer* pVertexBuffer = 0;

	static const int indexNum = 6;

public:
	SpriteRenderer();
	~SpriteRenderer(void);

	//頂点情報
	stVertex3D vtx[indexNum];

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

	XMMATRIX GetPosMatrix();
	XMMATRIX GetRotMatrix();
	XMMATRIX GetScaleMatrix();
};