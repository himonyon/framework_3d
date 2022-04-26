/*-----------------------------------------------------------

	フォントクラス
		フォントの出力
		Print関数を通して配列に毎フレームフォント情報を設定

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {

#define FONT_CHARACTER_MAX	(0x200)	//描画できる文字の数

	enum class eTextAlignment {
		Left,
		Right,
		Center
	};

	class Font : public Renderer {
	private:
		//Direct2D初期化用変数
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

		//呼ばれた文字列の登録
		void registerString(const WCHAR* string, UINT32 count);
		//フォントの作成
		bool Create(const WCHAR* fontname, float size);
		//テキストの表示基準位置の適用
		void AdjustTextAlignment();

	public:
		//初期化
		static bool Initialize(void* hdl);
		//破棄
		static void Destroy(void);

		Font();
		~Font();

		//コンポーネント処理
		void Execute() override;

		//文字の出力
		void RenderString();

		//出力文字の登録
		void Print(const WCHAR* string, ...);
		void Print(float left, float top, const WCHAR* string, ...);
		void Print(float left, float top, eTextAlignment alignment, const WCHAR* string, ...);

		//描画
		void Render();

		//Setter,Getter
		void SetRect();
		void SetRect(float left, float top, float right, float bottom);
		void SetRectWH(float x, float y, float width, float height);
		void SetColor(DWORD color);
		void SetFontStyle(float size, const WCHAR* fontname);
		void SetFontSize(float size);
		void SetTextAlignment(eTextAlignment alignment);
		float GetTextLength();
	};
}