#pragma once

//頂点構造体
struct stVertex2D
{
	float x, y;
	float r, g, b, a;
	float u, v;	
};

class Sprite {
public:
	Sprite* pParent;
	
	float posX;
	float posY;

	float sizeX;
	float sizeY;

	float scaleX;
	float scaleY;

	float rot = 0;

	stVertex2D vtx[4];

private:
	int renderPriority; //描画順
	bool sortEnable; //描画順が変更されたかどうか
	bool executeEnable; //実行管理
	bool renderEnable; //描画管理

private:
	ID3D11Buffer* pVertexBuffer;
	//テクスチャ
	ID3D11Resource* pTexture;
	ID3D11ShaderResourceView* pTextureView;

	static ID3D11Buffer* pConstantBuffer;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11SamplerState* pSamplerState;
	static ID3D11BlendState* pBlendState;
	static ID3D11DepthStencilState* pDepthStencilState;
	static ID3D11InputLayout* pInputLayout;
	static UINT	VertexStrides;
	static UINT	VertexOffsets;

	const static int VertexNum = 4;

public:
	static bool Initialize();
	static void Destroy();

	Sprite(float x, float y, float width, float height,  const WCHAR* texture_file, Sprite* parent = NULL, bool isRender = true, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f);
	~Sprite(void);

	void Render(void); //個別レンダー
	void Execute(void);
	
	void SetPosition(float x, float y);
	void SetColor(float r, float g, float b, float a);
	void SetScale(float x, float y);

	void SetRenderPriority(int value); //描画順位の設定
	int GetRenderPriority(); //描画順位の所得
	void SetSortEnable(bool flag);
	bool isSortEnable(); //描画順が変更さらたか
	void SetRenderEnable(bool flag); //描画状態の設定
	bool isRenderEnable(); //描画状態になっているか
	void SetExecuteEnable(bool flag); //実行状態の設定
	bool isExecuteEnable(); //実行状態になっているかどうか

	static void TextureCopy(Sprite* copy_to, Sprite* copy_from);

private:
	void SetVertexPosition();
};