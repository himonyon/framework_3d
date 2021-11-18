#pragma once

#define MaxFontNum (20)	//1フレームの描画できる文字列

class Font {
private:
	static ID2D1Factory* pD2d1Factory;
	static IDWriteFactory* pDWFactory;
	static ID2D1RenderTarget* pRenderTarget;
	static ID2D1SolidColorBrush* pBrush;

private:
	IDWriteTextFormat* pTextFormat = 0;
	static D2D1_RECT_F	rect;

	static Font* fonts[MaxFontNum];

	//変換用
	static WCHAR* FontStyle;
	static float Size;
	static DWORD Color; 
	static float PosX, PosY;
	static float RectL, RectR, RectT, RectB;
	static DWRITE_TEXT_ALIGNMENT Alignment;

	//実データ
	WCHAR* fontStyle;
	float size = 18.0f;
	bool isDraw = false;
	float posX, posY;
	float rectL, rectR, rectT, rectB;
	DWORD color = 0xffffffff;
	DWRITE_TEXT_ALIGNMENT alignment;
	WCHAR* ptr = NULL;
	UINT32 count = 0;

	WCHAR* allocPtr = NULL;
	WCHAR* currentPtr = NULL;

	static void registerString(int fontNum, const WCHAR* string, UINT32 count);
	bool Create(const WCHAR* fontname, int size);


public:
	static bool Initialize(void* hdl);
	static void Destroy(void);

	Font();
	~Font();

	static void RenderString();
	static void Print(const WCHAR* string, ...);
	static void Print(float left, float top, const WCHAR* string, ...);
	static void SetRect();
	static void SetRect(float left, float top, float right, float bottom);
	static void SetRectWH(float x, float y, float width, float height);
	static void SetColor(DWORD color);
	static void SetFontStyle(float size, const WCHAR* fontname);
	static void SetTextAlignment(DWRITE_TEXT_ALIGNMENT alignment);

	static void Render();

	static ID2D1RenderTarget* GetD2DRenderTarget();
};