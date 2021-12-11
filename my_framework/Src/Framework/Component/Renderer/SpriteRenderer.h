#pragma once
/*-----------------------------------------------------------

	SpriteRendererクラス
		スプライトを描画するクラス

-------------------------------------------------------------*/

class SpriteRenderer : public Component {
public:
	//描画対象スプライト
	noDel_ptr<Sprite> pRenderSprite = NULL;

	//画像の幅
	float sizeX = 0;
	float sizeY = 0;

	//頂点情報
	stVertex2D vtx[Sprite::VertexNum];

private:
	static ID3D11Buffer* pConstantBuffer;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11SamplerState* pSamplerState;
	static ID3D11BlendState* pBlendState;
	static ID3D11DepthStencilState* pDepthStencilState;
	static ID3D11InputLayout* pInputLayout;
	static UINT	VertexStrides;
	static UINT	VertexOffsets;

public:
	static bool Initialize();
	static void Destroy();

	SpriteRenderer();
	~SpriteRenderer(void);

	//コンポーネントの初期化
	void SetUpSpriteRenderer(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

	//コンポーネント処理
	void Execute() override;

	void SetSize(float width, float height);
	void SetColor(float r, float g, float b, float a);
	void SetColor(stColor4 color);

	void SetDefaultState(); //スプライトの状態を初期状態に戻す(ColorとUV)

	void SetRenderPriority(float value); //描画順位の設定
	float GetRenderPriority(); //描画順位の所得

private:
	void Render(void); //描画

	void SetVertexState();
};