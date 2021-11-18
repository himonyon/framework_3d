#pragma once
#include <functional>

/// <summary>
/// ���[�U�[����Ǝ��ݒ肳�ꂽInput�N���X
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

	//�f�o�C�X�̕ύX
	static void SetKeyboardDevice();
	static void SetJoystickDevice();
	static void SetBothDevice();
};