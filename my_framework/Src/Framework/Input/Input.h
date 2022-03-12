/*-----------------------------------------------------------

	Input�N���X
		InputConfig�ł܂Ƃ߂��z��𗘗p���Ċe�f�o�C�X�̓��͂����m����

-------------------------------------------------------------*/
#pragma once
#include <functional>


namespace MyFrameWork {

	//�f�o�C�X�̎��
	enum class eDeviceType {
		Both,
		Keyboard,
		Joystick,
		Max,
	};

	/// <summary>
	/// ���[�U�[����Ǝ��ݒ肳�ꂽInput�N���X
	/// </summary>
	/// 
	class Input {
	private:
		static eDeviceType currentDevice;

		//�f�o�C�X�ʂɏ����𕪂��邽�߂̊֐��|�C���^
		static std::function<bool(std::vector<int>*& inputs)> funcTrg;
		static std::function<bool(std::vector<int>*& inputs)> funcOn;
		static std::function<bool(std::vector<int>*& inputs)> funcRel;

		//�ړ��ʂŃ{�^����ݒ肷���,�}�C�i�X�l�ŌĂяo���l(txt�t�@�C���Őݒ�)�͈ړ��ʂ��}�C�i�X������Ԃ�
		static std::function<float(std::vector<int>*& inputs)> funcDX;
		static std::function<float(std::vector<int>*& inputs)> funcDY;

		//��x���������ꂽ������
		static bool BothDevTrg(std::vector<int>*& intuts);
		static bool KeyboardDevTrg(std::vector<int>*& intuts);
		static bool JoystickDevTrg(std::vector<int>*& intuts);

		//����Ă��邩����
		static bool BothDevOn(std::vector<int>*& inputs);
		static bool KeyboardDevOn(std::vector<int>*& inputs);
		static bool JoystickDevOn(std::vector<int>*& inputs);

		//������Ă����Ԃ��������ꂽ������
		static bool BothDevRel(std::vector<int>*& inputs);
		static bool KeyboardDevRel(std::vector<int>*& inputs);
		static bool JoystickDevRel(std::vector<int>*& inputs);

		//X�����ւ̓��͗ʂ����m(�K��@�P)
		static float BothDevDX(std::vector<int>*& inputs);
		static float KeyboardDevDX(std::vector<int>*& inputs);
		static float JoystickDevDX(std::vector<int>*& inputs);

		//Y�����ւ̓��͗ʂ����m(�K��@�P)
		static float BothDevDY(std::vector<int>*& inputs);
		static float KeyboardDevDY(std::vector<int>*& inputs);
		static float JoystickDevDY(std::vector<int>*& inputs);


	public:
		static float GetDX(std::vector<int>*& inputs);
		static float GetDY(std::vector<int>*& inputs);
		static bool Trg(std::vector<int>*& inputs);
		static bool On(std::vector<int>*& inputs);
		static bool Rel(std::vector<int>*& inputs);

		//�}�E�X�������Ă��邩
		static bool IsMouseMove();

		static eDeviceType GetDeviceType() { return currentDevice; }

		//�f�o�C�X�̕ύX
		static void SetKeyboardDevice();
		static void SetJoystickDevice();
		static void SetBothDevice();
	};
}