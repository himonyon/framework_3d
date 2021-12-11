/*-----------------------------------------------------------

	フォントクラス
		フォントの出力
		Print関数を通して配列に毎フレームフォント情報を設定

-------------------------------------------------------------*/
#pragma once

#define FONT_CHARACTER_MAX	(0x200)	//描画できる文字の数

enum class eTextAlignment {
	Left,
	Right,
	Center
};

class Font : public Component {
private:
	static ID2D1Factory* pD2d1Factory;
	static IDWriteFactory* pDWFactory;
	static ID2D1RenderTarget* pRenderTarget;

private:
	IDWriteTextFormat* pTextFormat = 0;
	ID2D1SolidColorBrush* pBrush;
	D2D1_RECT_F	rect;

	const float reciprocal = 1.0f / 255.0f; //色変換用

	//実データ
	std::wstring fontStyle;
	float size;
	float posX, posY;
	float rectL, rectR, rectT, rectB;
	DWORD color;
	eTextAlignment alignment;
	WCHAR* ptr;
	UINT32 count;

	WCHAR* allocPtr = NULL;
	WCHAR* currentPtr = NULL;

	void registerString(const WCHAR* string, UINT32 count);
	bool Create(const WCHAR* fontname, float size);

	void AdjustTextAlignment();
	float GetTextLength();

public:
	static bool Initialize(void* hdl);
	static void Destroy(void);

	Font();
	~Font();

	void Execute() override;

	void RenderString();
	void Print(const WCHAR* string, ...);
	void Print(float left, float top, const WCHAR* string, ...);
	void Print(float left, float top, eTextAlignment alignment, const WCHAR* string, ...);
	void SetRect();
	void SetRect(float left, float top, float right, float bottom);
	void SetRectWH(float x, float y, float width, float height);
	void SetColor(DWORD color);
	void SetFontStyle(float size, const WCHAR* fontname);
	void SetFontSize(float size);
	void SetTextAlignment(eTextAlignment alignment);
	void Render();

	static ID2D1RenderTarget* GetD2DRenderTarget();
};