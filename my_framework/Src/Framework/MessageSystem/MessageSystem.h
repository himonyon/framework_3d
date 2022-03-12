#pragma once
/*-----------------------------------------------------------

	MessageSystem�N���X(�Q�[���I�u�W�F�N�g���N���X)
		�R���|�[�l���g�Ƀ��b�Z�[�W�𑗐M
		���b�Z�[�W����M

-------------------------------------------------------------*/

namespace MyFrameWork {

	class Component;

	class MessageSystem {
	public:
		MessageSystem() {};
		virtual ~MessageSystem() {};

		//�R���|�[�l���g�փ��b�Z�[�W�𑗂�
		static void SendMessageToCom(noDel_ptr<Component> com, std::wstring str);
		//���b�Z�[�W�̍폜
		void DeleteMessage(std::wstring str);

	public:
		std::vector<std::wstring> messages;
	};
}