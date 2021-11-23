/*-----------------------------------------------------------

	ゲームオブジェクト2Dクラス
		画像を扱うゲームオブジェクト

-------------------------------------------------------------*/
#pragma once

class GameObject2D : public GameObject {
public:
	//描画対象スプライト
	noDel_ptr<Sprite> pRenderSprite = NULL;

	//画像の幅
	float sizeX;
	float sizeY;

	//カラー
	stColor4 color[Sprite::VertexNum];

private:
	int renderPriority = 0; //描画順
	bool sortEnable = true; //描画順が変更されたかどうか

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

	GameObject2D(float x, float y, float width, float height, noDel_ptr<Sprite> sprite, bool isRender = true, noDel_ptr<GameObject> parent = NULL);
	~GameObject2D(void);

	void Render(void) override; //個別レンダー

	void SetSize(float width, float height);
	void SetColor(float r, float g, float b, float a);
	void SetColor(stColor4 color);
	void SetRotation(float rot);

	void SetRenderPriority(int value); //描画順位の設定
	int GetRenderPriority(); //描画順位の所得
	void SetSortEnable(bool flag);
	bool isSortEnable(); //描画順が変更さらたか

private:
	void SetVertexState();
};
