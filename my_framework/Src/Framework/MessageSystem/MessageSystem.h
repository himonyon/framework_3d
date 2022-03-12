#pragma once
/*-----------------------------------------------------------

	MessageSystemクラス(ゲームオブジェクト基底クラス)
		コンポーネントにメッセージを送信
		メッセージを受信

-------------------------------------------------------------*/

namespace MyFrameWork {

	class Component;

	class MessageSystem {
	public:
		MessageSystem() {};
		virtual ~MessageSystem() {};

		//コンポーネントへメッセージを送る
		static void SendMessageToCom(noDel_ptr<Component> com, std::wstring str);
		//メッセージの削除
		void DeleteMessage(std::wstring str);

	public:
		std::vector<std::wstring> messages;
	};
}