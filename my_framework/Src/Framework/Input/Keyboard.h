#pragma once

class Keyboard {
public:
	//�L�[�{�[�h(DIK)
	static BYTE GetTrgKeyType(); //�����ꂽ�L�[��DirectInput�L�[���ʃR�[�h�̎�ނ�ς���
	static BYTE GetOnKeyType(); //�����ꂽ�L�[��DirectInput�L�[���ʃR�[�h�̎�ނ�ς���
	static BYTE GetRelKeyType(); //�����ꂽ�L�[��DirectInput�L�[���ʃR�[�h�̎�ނ�ς���
	static bool On(BYTE key);
	static bool Trg(BYTE key);
	static bool Rel(BYTE key);
};