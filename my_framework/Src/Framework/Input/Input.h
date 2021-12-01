/*-----------------------------------------------------------

	Input�N���X
		InputConfig�ł܂Ƃ߂��z��𗘗p���Ċe�f�o�C�X�̓��͂����m����

-------------------------------------------------------------*/
#pragma once
#include <functional>

/// <summary>
/// ���[�U�[����Ǝ��ݒ肳�ꂽInput�N���X
/// </summary>
/// 

class Input {
private:
	enum class eDeviceType {
		Keyboard,
		Joystick,
		Both
	};
	static eDeviceType currentDevice;

	static std::function<bool(std::vector<int>& inputs)> funcTrg;
	static std::function<bool(std::vector<int>& inputs)> funcOn;
	static std::function<bool(std::vector<int>& inputs)> funcRel;

	//�ړ��ʂŃ{�^����ݒ肷��ۂ̓}�C�i�X�l�͌Ăяo���l(txt�t�@�C���Őݒ�)���}�C�i�X�ɂ���
	static std::function<float(std::vector<int>& inputs)> funcDX;
	static std::function<float(std::vector<int>& inputs)> funcDY;

	static bool BothDevTrg(std::vector<int>& intuts);
	static bool KeyboardDevTrg(std::vector<int>& intuts);
	static bool JoystickDevTrg(std::vector<int>& intuts);

	static bool BothDevOn(std::vector<int>& inputs);
	static bool KeyboardDevOn(std::vector<int>& inputs);
	static bool JoystickDevOn(std::vector<int>& inputs);

	static bool BothDevRel(std::vector<int>& inputs);
	static bool KeyboardDevRel(std::vector<int>& inputs);
	static bool JoystickDevRel(std::vector<int>& inputs);

	static float BothDevDX(std::vector<int>& inputs);
	static float KeyboardDevDX(std::vector<int>& inputs);
	static float JoystickDevDX(std::vector<int>& inputs);

	static float BothDevDY(std::vector<int>& inputs);
	static float KeyboardDevDY(std::vector<int>& inputs);
	static float JoystickDevDY(std::vector<int>& inputs);


public:
	//�{�^���̏ꍇbtnPlus��true�Ȃ�P�Afalse-1�Ȃ��Ԃ�
	static float GetDX(std::vector<int>& inputs);
	//�{�^���̏ꍇbtnPlus��true�Ȃ�P�Afalse-1�Ȃ��Ԃ�
	static float GetDY(std::vector<int>& inputs);
	static bool Trg(std::vector<int>& inputs);
	static bool On(std::vector<int>& inputs);
	static bool Rel(std::vector<int>& inputs);

	//�}�E�X�������Ă��邩
	static bool IsMouseMove();

	//�f�o�C�X�̕ύX
	static void SetKeyboardDevice();
	static void SetJoystickDevice();
	static void SetBothDevice();
};