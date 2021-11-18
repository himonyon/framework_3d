#pragma once

class GameObject2D : public GameObject {
public:
	//•`‰æ‘ÎÛƒXƒvƒ‰ƒCƒg
	noDel_ptr<Sprite> pRenderSprite = NULL;

	//‰æ‘œ‚Ì•
	float sizeX;
	float sizeY;

	//ƒJƒ‰[
	float col_r[Sprite::VertexNum], col_g[Sprite::VertexNum], col_b[Sprite::VertexNum], col_a[Sprite::VertexNum];

private:
	int renderPriority = 0; //•`‰æ‡
	bool sortEnable = true; //•`‰æ‡‚ª•ÏX‚³‚ê‚½‚©‚Ç‚¤‚©

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

	void Render(void) override; //•`‰æˆ—
	void Execute(void) override; //ˆ—

	void SetColor(float r, float g, float b, float a);
	void SetRotation(float rot);

	void SetRenderPriority(int value); //•`‰æ‡ˆÊ‚Ìİ’è
	int GetRenderPriority(); //•`‰æ‡ˆÊ‚ÌŠ“¾
	void SetSortEnable(bool flag);
	bool isSortEnable(); //•`‰æ‡‚ª•ÏX‚³‚ç‚½‚©

private:
	void SetVertexState();
};
