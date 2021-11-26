/*-----------------------------------------------------------

	フォントクラス
		フォントの出力
		全シーン共通で利用する配列を用意している
		Print関数を通して配列に毎フレームフォント情報を設定

-------------------------------------------------------------*/
#pragma once

#define MaxFontNum (64)	//1フレームの描画できる文字列

enum class eTextAlignment {
	Left,
	Right,
	Center
};

class Font {
private:
	static ID2D1Factory* pD2d1Factory;
	static IDWriteFactory* pDWFactory;
	static ID2D1RenderTarget* pRenderTarget;
	static ID2D1SolidColorBrush* pBrush;

public:
	//変換用
	static WCHAR* FontStyle;
	static float Size;
	static DWORD Color;
	static float PosX, PosY;
	static float RectL, RectR, RectT, RectB;
	static eTextAlignment Alignment;

private:
	IDWriteTextFormat* pTextFormat = 0;
	static D2D1_RECT_F	rect;

	static Font* fonts[MaxFontNum];

	//実データ
	WCHAR* fontStyle;
	float size = 18.0f;
	bool isDraw = false;
	float posX, posY;
	float rectL, rectR, rectT, rectB;
	DWORD color = 0xffffffff;
	eTextAlignment alignment;
	WCHAR* ptr = NULL;
	UINT32 count = 0;

	WCHAR* allocPtr = NULL;
	WCHAR* currentPtr = NULL;

	static void registerString(int fontNum, const WCHAR* string, UINT32 count);
	bool Create(const WCHAR* fontname, int size);

	static void AdjustTextAlignment(Font* font);
	static float GetTextLength(Font* font);

public:
	static bool Initialize(void* hdl);
	static void Destroy(void);

	Font();
	~Font();

	static void RenderString();
	static void Print(float left, float top, const WCHAR* string, ...);
	static void Print(float left, float top, eTextAlignment alignment, const WCHAR* string, ...);
	static void SetRect();
	static void SetRect(float left, float top, float right, float bottom);
	static void SetRectWH(float x, float y, float width, float height);
	static void SetColor(DWORD color);
	static void SetFontStyle(float size, const WCHAR* fontname);
	static void SetFontSize(float size);
	static void SetTextAlignment(eTextAlignment alignment);

	static void Render();

	static ID2D1RenderTarget* GetD2DRenderTarget();
};