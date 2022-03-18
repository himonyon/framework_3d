/*-----------------------------------------------------------

	マウスクラス
		マウスの入力を受け付ける

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {

	//マウスの入力ナンバー
	//キーボードの数字と被らないため大きめに設定
#define MOUSE_L (501)
#define MOUSE_R (504)
#define MOUSE_WHEEEl (502)
#define MOUSE_DXY (502)

	class Mouse {
	public:
		static bool On(int mouse);
		static bool Trg(int mouse);
		static bool Rel(int mouse);

		static int GetX();
		static int GetY();
		static int GetR();
		static int GetDX();
		static int GetDY();

	private:
		static int x;
		static int y;
		static int r;
		static int on;
		static int trg;
		static int rel;

	public:
		//マウスの状態を設定
		static void SetMouseStatus(void);

		//マウスのポジションをセットする
		static void SetMousePosition(int _x, int _y);
	};
}