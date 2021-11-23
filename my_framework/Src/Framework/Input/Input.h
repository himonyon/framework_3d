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
	static std::function<bool(std::vector<int>& inputs)> funcTrg;
	static std::function<bool(std::vector<int>& inputs)> funcOn;
	static std::function<bool(std::vector<int>& inputs)> funcRel;

	static bool BothDevTrg(std::vector<int>& intuts);
	static bool KeyboardDevTrg(std::vector<int>& intuts);
	static bool JoystickDevTrg(std::vector<int>& intuts);

	static bool BothDevOn(std::vector<int>& inputs);
	static bool KeyboardDevOn(std::vector<int>& inputs);
	static bool JoystickDevOn(std::vector<int>& inputs);

	static bool BothDevRel(std::vector<int>& inputs);
	static bool KeyboardDevRel(std::vector<int>& inputs);
	static bool JoystickDevRel(std::vector<int>& inputs);


public:
	static bool Trg(std::vector<int>& inputs);
	static bool On(std::vector<int>& inputs);
	static bool Rel(std::vector<int>& inputs);

	//�f�o�C�X�̕ύX
	static void SetKeyboardDevice();
	static void SetJoystickDevice();
	static void SetBothDevice();
};