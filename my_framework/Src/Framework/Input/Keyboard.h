/*-----------------------------------------------------------

	�L�[�{�[�h�N���X
		�L�[�{�[�h�̓��͂��󂯕t����

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {

	class Keyboard {
	public:
		//�L�[�{�[�h(DIK)
		static int GetTrgKeyType(); //�����ꂽ�L�[��DirectInput�L�[���ʃR�[�h�̎�ނ�ς���
		static int GetOnKeyType(); //�����ꂽ�L�[��DirectInput�L�[���ʃR�[�h�̎�ނ�ς���
		static int GetRelKeyType(); //�����ꂽ�L�[��DirectInput�L�[���ʃR�[�h�̎�ނ�ς���
		static bool On(int key);
		static bool Trg(int key);
		static bool Rel(int key);
	};
}