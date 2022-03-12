#include "../../framework.h"
#include "../../environment.h"


ID2D1Factory* DebugFont::pD2d1Factory = 0;
IDWriteFactory* DebugFont::pDWFactory = 0;
ID2D1RenderTarget* DebugFont::pRenderTarget = 0;
ID2D1SolidColorBrush* DebugFont::pBrush;
D2D1_RECT_F	DebugFont::rect = { 0 };

DebugFont* DebugFont::fonts[] = {0};

DWORD DebugFont::Color = 0xffffffff;
DWRITE_TEXT_ALIGNMENT DebugFont::Alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
WCHAR font[] = L"Meiryo";
WCHAR* DebugFont::FontStyle = font;
float DebugFont::Size = 18.0f;
float DebugFont::PosX = 0, DebugFont::PosY = 0;
float DebugFont::RectL = 0, DebugFont::RectR = WINDOW_WIDTH, DebugFont::RectT = 0, DebugFont::RectB = WINDOW_HEIGHT;

bool DebugFont::Initialize(void* hdl) {
	HWND hWnd = (HWND)hdl;

	HRESULT hr;

	//ID2D1Factoryの生成
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2d1Factory);
	if (FAILED(hr))
	{
		Destroy();
		return false;
	}

	//IDWriteFactoryの生成
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWFactory));
	if (FAILED(hr))
	{
		Destroy();
		return false;
	}

	//ID2D1HwndRenderTargetの生成
	D2D1_SIZE_U renderTargetSize = { (UINT32)WINDOW_WIDTH, (UINT32)WINDOW_HEIGHT };
	D2D1_PIXEL_FORMAT pixelFormat;
	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTargetProperties;

	pixelFormat.format = DXGI_FORMAT_UNKNOWN;
	pixelFormat.alphaMode = D2D1_ALPHA_MODE_UNKNOWN;

	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	renderTargetProperties.pixelFormat = pixelFormat;
	renderTargetProperties.dpiX = 0;
	renderTargetProperties.dpiY = 0;
	renderTargetProperties.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	renderTargetProperties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	hwndRenderTargetProperties.hwnd = hWnd;
	hwndRenderTargetProperties.pixelSize = renderTargetSize;
	hwndRenderTargetProperties.presentOptions = D2D1_PRESENT_OPTIONS_RETAIN_CONTENTS;


	IDXGISurface* pBackBuffer = NULL;
	hr = Direct3D::getSwapChain()->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (FAILED(hr))
	{
		Destroy();
		return false;
	}

	UINT dpi = GetDpiForWindow(hWnd);
	FLOAT dpiX = (FLOAT)dpi;
	FLOAT dpiY = (FLOAT)dpi;

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

	hr = pD2d1Factory->CreateDxgiSurfaceRenderTarget(pBackBuffer, &props, &pRenderTarget);
	if (FAILED(hr))
	{
		Destroy();
		return false;
	}

	//ブラシの生成
	hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1), &pBrush);
	if (hr != S_OK)
	{
		return false;
	}


	for (int i = 0; i < MaxFontNum; i++) {
		fonts[i] = new DebugFont();
	}

	rect.left = RectL;
	rect.top = RectT;
	rect.right = RectR;
	rect.bottom = RectB;

	return true;
}
void DebugFont::Destroy(void) {
	SAFE_RELEASE(pBrush);
	SAFE_RELEASE(pRenderTarget);
	SAFE_RELEASE(pDWFactory);
	SAFE_RELEASE(pD2d1Factory);
}

DebugFont::DebugFont() {
	size = Size;

	allocPtr = new WCHAR[FONT_CHARACTER_MAX];
	currentPtr = allocPtr;

	
	//フォントの作成
	if (Create(FontStyle, (int)size) == false)
	{
		return;
	}

	fontStyle = FontStyle;
}

DebugFont::~DebugFont() {
	delete[] allocPtr;
	SAFE_RELEASE(pTextFormat);
}

void DebugFont::registerString(int fontNum, const WCHAR* string, UINT32 count) {
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4302)
	if (string == NULL)return;
	if (fonts[fontNum]->fontStyle != FontStyle || fonts[fontNum]->size != Size) {
		fonts[fontNum]->size = Size;
		fonts[fontNum]->fontStyle = FontStyle;
		fonts[fontNum]->Create(FontStyle, (int)Size);
	}
	fonts[fontNum]->posX = PosX;
	fonts[fontNum]->posY = PosY;
	fonts[fontNum]->rectL = RectL;
	fonts[fontNum]->rectT = RectT;
	fonts[fontNum]->rectR = RectR;
	fonts[fontNum]->rectB = RectB;
	fonts[fontNum]->alignment = Alignment;
	fonts[fontNum]->color = Color;
	fonts[fontNum]->ptr = fonts[fontNum]->currentPtr;
	fonts[fontNum]->count = count;
	wcsncpy_s((WCHAR*)fonts[fontNum]->currentPtr, FONT_CHARACTER_MAX - (reinterpret_cast<int>(fonts[fontNum]->currentPtr) - reinterpret_cast<int>(fonts[fontNum]->allocPtr)), string, count);
	fonts[fontNum]->currentPtr += count;
#pragma warning(pop)
}

void DebugFont::RenderString() {

	static float reciprocal = 1.0f / 255.0f;
	for (int i = 0; i < MaxFontNum; i++) {
		if (fonts[i]->pTextFormat == NULL)return;
		if (fonts[i]->isDraw == false) {
			return;
		}

		//描画位置の設定
		rect.left = fonts[i]->rectL + fonts[i]->posX;
		rect.top = fonts[i]->rectT + fonts[i]->posY;
		rect.right = fonts[i]->rectR;
		rect.bottom = fonts[i]->rectB;

		fonts[i]->pTextFormat->SetTextAlignment(fonts[i]->alignment);

		//描画色の設定
		Color = fonts[i]->color;

		D2D1_COLOR_F col;
		col.r = (float)((Color & 0x00ff0000) >> 16) * reciprocal;
		col.g = (float)((Color & 0x0000ff00) >> 8) * reciprocal;
		col.b = (float)((Color & 0x000000ff) >> 0) * reciprocal;
		col.a = (float)((Color & 0xff000000) >> 24) * reciprocal;
		pBrush->SetColor(&col);

		//テキストの描画
		pRenderTarget->DrawText(
			fonts[i]->ptr,
			fonts[i]->count,
			fonts[i]->pTextFormat,
			rect,
			pBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE
		);
		fonts[i]->currentPtr = fonts[i]->allocPtr;
		fonts[i]->isDraw = false;
	}

	//描画位置のリセット
	RectL = 0;
	RectT = 0;
	RectR = WINDOW_WIDTH;
	RectB = WINDOW_HEIGHT;

	return;
}

bool DebugFont::Create(const WCHAR* fontname, int size) {
	HRESULT hr;

	SAFE_RELEASE(pTextFormat);

	//テキストフォーマットの生成
	hr = pDWFactory->CreateTextFormat(
		fontname,
		NULL,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(float)size,
		L"",
		&pTextFormat
	);

	if (hr != S_OK)
	{
		return false;
	}

	return true;
}

void DebugFont::Print(const WCHAR* string, ...) {
	if (string == NULL)return;
	for (int i = 0; i < MaxFontNum; i++) {
		if (fonts[i]->isDraw == false) {
			fonts[i]->isDraw = true;
			va_list	va;
			va_start(va, string);
			WCHAR buf[0x100];
			vswprintf_s(buf, string, va);
			va_end(va);

			registerString(i, buf, (UINT32)wcslen(buf));
			return;
		}
	}
}

void DebugFont::Print(float left, float top, const WCHAR* string, ...) {
	if (string == NULL)return;
	for (int i = 0; i < MaxFontNum; i++) {
		if (fonts[i]->isDraw == false) {
			fonts[i]->isDraw = true;
			PosX = left;
			PosY = top;
			va_list	va;
			va_start(va, string);
			WCHAR buf[0x100];
			vswprintf_s(buf, string, va);
			va_end(va);

			registerString(i, buf, (UINT32)wcslen(buf));
			return;
		}
	}
}

void DebugFont::Render() {
	//描画開始(Direct2D)
	pRenderTarget->BeginDraw();

	//フォントの描画
	RenderString();

	//描画終了(Direct2D)
	pRenderTarget->EndDraw();
}

void DebugFont::SetRect() {
	RectL = 0;
	RectT = 0;
	RectR = WINDOW_WIDTH;
	RectB = WINDOW_HEIGHT;
}
void DebugFont::SetRect(float left, float top, float right, float bottom) {
	RectL = left;
	RectT = top;
	RectR = right;
	RectB = bottom;
}
void DebugFont::SetRectWH(float x, float y, float width, float height) {
	RectL = x - width/2;
	RectT = y - height/2;
	RectR = x + width/2;
	RectB = y + height/2;
}

void DebugFont::SetColor(DWORD color) {
	Color = color;
}

void DebugFont::SetFontStyle(const WCHAR* fontName, float size) {
	Size = size;
	FontStyle = (WCHAR*)fontName;
}

void DebugFont::SetFontSize(float size) {
	Size = size;
}

void DebugFont::SetTextAlignment(DWRITE_TEXT_ALIGNMENT textAlignment) {
	switch (textAlignment) {
	case DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING:
		Alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
		break;
	case DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING:
		Alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING;
		break;
	case DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER:
		Alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER;
		break;
	}
}

ID2D1RenderTarget* DebugFont::GetD2DRenderTarget() {
	return pRenderTarget;
}