#include "../../../../framework.h"
#include "../../../../environment.h"

ID2D1Factory* Font::pD2d1Factory = 0;
IDWriteFactory* Font::pDWFactory = 0;
ID2D1RenderTarget* Font::pRenderTarget = 0;


bool Font::Initialize(void* hdl) {
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

	

	return true;
}
void Font::Destroy(void) {
	SAFE_RELEASE(pRenderTarget);
	SAFE_RELEASE(pDWFactory);
	SAFE_RELEASE(pD2d1Factory);
}

Font::Font() : Component(eComponentType::Font) {
	size = 18.0f;

	rect.left = 0;
	rect.right = WINDOW_WIDTH;
	rect.top = 0;
	rect.bottom = WINDOW_HEIGHT;

	//実データ
	fontStyle = L"Meiryo";
	size = 18.0f;
	posX = 0, posY = 0;
	rectL = 0, rectR = WINDOW_WIDTH, rectT = 0, rectB = WINDOW_HEIGHT;
	color = 0xffffffff;
	alignment = eTextAlignment::Left;
	ptr = NULL;
	count = 0;

	allocPtr = new WCHAR[FONT_CHARACTER_MAX];
	currentPtr = allocPtr;

	//ブラシの生成
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1), &pBrush);

	static float reciprocal = 1.0f / 255.0f; D2D1_COLOR_F col;
	col.r = (float)((color & 0x00ff0000) >> 16) * reciprocal;
	col.g = (float)((color & 0x0000ff00) >> 8) * reciprocal;
	col.b = (float)((color & 0x000000ff) >> 0) * reciprocal;
	col.a = (float)((color & 0xff000000) >> 24) * reciprocal;
	pBrush->SetColor(&col);
	
	//フォントの作成
	if (Create(fontStyle.c_str(), size) == false)
	{
		return;
	}
}

Font::~Font() {
	delete[] allocPtr;
	SAFE_RELEASE(pTextFormat);
	SAFE_RELEASE(pBrush);
}

void Font::Execute() {
	Render();
}

void Font::registerString(const WCHAR* string, UINT32 count) {
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4302)
	if (string == NULL)return;
	ptr = currentPtr;
	count = count;
	wcsncpy_s((WCHAR*)currentPtr, FONT_CHARACTER_MAX - (reinterpret_cast<int>(currentPtr) - reinterpret_cast<int>(allocPtr)), string, count);
	currentPtr += count;
	this->count = count;
#pragma warning(pop)
}

void Font::RenderString() {
	if (pTextFormat == NULL)return;

	//描画位置の設定
	AdjustTextAlignment();
	rect.left = rectL + posX;
	rect.top = rectT + posY;
	rect.right = rectR;
	rect.bottom = rectB;

	//テキストの描画
	pRenderTarget->DrawText(
		ptr,
		count,
		pTextFormat,
		rect,
		pBrush,
		D2D1_DRAW_TEXT_OPTIONS_NONE
	);
	currentPtr = allocPtr;
}

void Font::AdjustTextAlignment() {
	if (alignment == eTextAlignment::Center) {
		posX -= GetTextLength() / 2;
	}else if (alignment == eTextAlignment::Right) {
		posX -= GetTextLength();
	}
}

float Font::GetTextLength() {
	int count = 0;
	float length = 0;
	float maxLength = 0; //複数行ある時に一番長い行を対象とする
	while (ptr[count] != '\0') {
		if (ptr[count] == '\n') {
			if (maxLength < length) maxLength = length;
			length = 0;
			count++;
			continue;
		}
		length += ptr[count] < 0xa7 ? 0.5f : 1;
		count++;
	}
	if (maxLength == 0) maxLength = length;
	return maxLength * size;
}

bool Font::Create(const WCHAR* fontname, float size) {
	HRESULT hr;

	SAFE_RELEASE(pTextFormat);

	//テキストフォーマットの生成
	hr = pDWFactory->CreateTextFormat(
		fontname,
		NULL,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		L"",
		&pTextFormat
	);

	if (hr != S_OK)
	{
		return false;
	}

	return true;
}

void Font::Print(float left, float top, eTextAlignment alignment, const WCHAR* string, ...) {
	if (string == NULL)return;
	posX = left;
	posY = top;
	alignment = alignment;
	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)wcslen(buf));
}

void Font::Print(float left, float top, const WCHAR* string, ...) {
	if (string == NULL)return;
	posX = left;
	posY = top;
	va_list	va;
	va_start(va, string);
	WCHAR buf[0x100];
	vswprintf_s(buf, string, va);
	va_end(va);

	registerString(buf, (UINT32)wcslen(buf));
}

void Font::Render() {
	//描画開始(Direct2D)
	pRenderTarget->BeginDraw();

	//フォントの描画
	RenderString();

	//描画終了(Direct2D)
	pRenderTarget->EndDraw();
}

void Font::SetRect() {
	rectL = 0;
	rectT = 0;
	rectR = WINDOW_WIDTH;
	rectB = WINDOW_HEIGHT;
}
void Font::SetRect(float rectL, float rectT, float rectR, float rectB) {
	rectL = rectL;
	rectT = rectT;
	rectR = rectR;
	rectB = rectB;
}
void Font::SetRectWH(float x, float y, float width, float height) {
	rectL = x - width/2;
	rectT = y - height / 2;
	rectR = x + width / 2;
	rectB = y + height/2;
}

void Font::SetColor(DWORD color) {
	//描画色の設定
	if (this->color != color)
	{
		this->color = color;

		D2D1_COLOR_F col;
		col.r = (float)((color & 0x00ff0000) >> 16) * reciprocal;
		col.g = (float)((color & 0x0000ff00) >> 8) * reciprocal;
		col.b = (float)((color & 0x000000ff) >> 0) * reciprocal;
		col.a = (float)((color & 0xff000000) >> 24) * reciprocal;
		pBrush->SetColor(&col);
	}
}

void Font::SetFontStyle(float size, const WCHAR* fontName) {
	if (this->size != size || this->fontStyle != (WCHAR*)fontName) {
		this->size = size;
		this->fontStyle = (WCHAR*)fontName;
		Create(fontStyle.c_str(), size);
	}
}

void Font::SetFontSize(float size) {
	if (this->size != size) {
		this->size = size;
		Create(fontStyle.c_str(), size);
	}
}

void Font::SetTextAlignment(eTextAlignment textAlignment) {
	switch (textAlignment) {
	case eTextAlignment::Left:
		alignment = eTextAlignment::Left;
		break;
	case eTextAlignment::Right:
		alignment = eTextAlignment::Right;
		break;
	case eTextAlignment::Center:
		alignment = eTextAlignment::Center;
		break;
	}
}

ID2D1RenderTarget* Font::GetD2DRenderTarget() {
	return pRenderTarget;
}