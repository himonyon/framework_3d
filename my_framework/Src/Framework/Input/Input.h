#pragma once
#include <functional>

/// <summary>
/// ユーザーから独自設定されたInputクラス
/// </summary>
/// 

class Input {
private:
	static std::function<bool(int(&inputs)[2])> funcTrg;
	static std::function<bool(int(&inputs)[2])> funcOn;
	static std::function<bool(int(&inputs)[2])> funcRel;

	static bool BothDevTrg(int(&inputs)[2]);
	static bool KeyboardDevTrg(int(&inputs)[2]);
	static bool JoystickDevTrg(int(&inputs)[2]);

	static bool BothDevOn(int(&inputs)[2]);
	static bool KeyboardDevOn(int(&inputs)[2]);
	static bool JoystickDevOn(int(&inputs)[2]);

	static bool BothDevRel(int(&inputs)[2]);
	static bool KeyboardDevRel(int(&inputs)[2]);
	static bool JoystickDevRel(int(&inputs)[2]);


public:
	static bool Trg(int(&inputs)[2]);
	static bool On(int(&inputs)[2]);
	static bool Rel(int(&inputs)[2]);

	//デバイスの変更
	static void SetKeyboardDevice();
	static void SetJoystickDevice();
	static void SetBothDevice();
};