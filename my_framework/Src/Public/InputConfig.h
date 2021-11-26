/*-----------------------------------------------------------

	���̓R���t�B�O�N���X
		���͂���{�^����p�r�ʂɂ܂Ƃ߂�N���X
		�e�L�X�g�t�@�C�����炻�ꂼ��̗p�r�ɑΉ�����{�^�����擾

-------------------------------------------------------------*/
#pragma once

/// <summary>
/// �L�[�{�[�h�A�W���C�X�e�B�b�N�̃��[�U�[�ݒ�p�N���X
/// �L�[�{�[�h�ƃW���C�X�e�B�b�N�̊e�{�^���ɋ��ʂ̖�������������
/// </summary>

class InputConfig {
public:
	static std::vector<int> decide;
	static std::vector<int> cancel;
	static std::vector<int> cursorUp;
	static std::vector<int> cursorDown;
	static std::vector<int> cursorRight;
	static std::vector<int> cursorLeft;
	static std::vector<int> moveX;
	static std::vector<int> moveY;

	static bool SetUpConfig();
};