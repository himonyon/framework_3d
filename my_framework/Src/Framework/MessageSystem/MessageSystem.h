#pragma once
/*-----------------------------------------------------------

	MessageSystemクラス(ゲームオブジェクト基底クラス)
		オブジェクトにメッセージを送信
		メッセージを受信

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