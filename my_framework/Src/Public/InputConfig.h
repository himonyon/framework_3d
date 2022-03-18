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

namespace MyFrameWork {

	class InputConfig {
	public:
		static std::unordered_map<std::string, std::vector<int>*> input;

		static bool SetUpConfig();
		static void DestroyConfig();
	};
}