/*-----------------------------------------------------------

	キーボードクラス
		キーボードの入力を受け付ける

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {

	class Keyboard {
	public:
		//キーボード(DIK)
		static int GetTrgKeyType(); //押されたキーのDirectInputキー識別コードの種類を変えす
		static int GetOnKeyType(); //押されたキーのDirectInputキー識別コードの種類を変えす
		static int GetRelKeyType(); //押されたキーのDirectInputキー識別コードの種類を変えす
		static bool On(int key);
		static bool Trg(int key);
		static bool Rel(int key);
	};
}