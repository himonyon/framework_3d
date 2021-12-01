#pragma once
/*-----------------------------------------------------------

	MessageSystem�N���X(�Q�[���I�u�W�F�N�g���N���X)
		�I�u�W�F�N�g�Ƀ��b�Z�[�W�𑗐M
		���b�Z�[�W����M

-------------------------------------------------------------*/

class Component;

class MessageSystem {
public:
	MessageSystem() {};
	virtual ~MessageSystem() {};

	static void SendMessageToCom(noDel_ptr<Component> com, std::wstring str);
	void DeleteMessage(std::wstring str);

public:
	std::vector<std::wstring> messages;
};