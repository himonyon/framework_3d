/*-----------------------------------------------------------

	マウスクラス
		マウスの入力を受け付ける

-------------------------------------------------------------*/
#pragma once

class Mouse {
public:
	//マウス(L(1) R(4) R(2))
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