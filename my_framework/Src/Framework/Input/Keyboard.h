#pragma once

class Keyboard {
public:
	//キーボード(DIK)
	static BYTE GetTrgKeyType(); //押されたキーのDirectInputキー識別コードの種類を変えす
	static BYTE GetOnKeyType(); //押されたキーのDirectInputキー識別コードの種類を変えす
	static BYTE GetRelKeyType(); //押されたキーのDirectInputキー識別コードの種類を変えす
	static bool On(BYTE key);
	static bool Trg(BYTE key);
	static bool Rel(BYTE key);
};