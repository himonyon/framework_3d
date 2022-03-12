#include "../../framework.h"
#include "../../environment.h"

std::unordered_map<std::string, std::vector<int>*> InputConfig::input;

bool InputConfig::SetUpConfig() {
	char _key[64] = { 0 };
	//�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	_wfopen_s(&fp, L"Data/Input/InputConfig.txt", L"rt");
	if (fp == NULL) return false;

	std::vector<int>* temp_vec = nullptr;

	//�܂��͒��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));
		//�{�^����
		if (strcmp(_key, "name") == 0) {
			temp_vec = new std::vector<int>;
			fscanf_s(fp, "%s", _key, (int)sizeof(_key));
			std::string str = std::string(_key);
			input[str] = temp_vec;
		}
		else if (strcmp(_key, "end") == 0) {
			temp_vec = nullptr;
		}
		//�e�R���e�i�ɐ������i�[����
		else {
			int temp_i = std::stoi(_key);
			temp_vec->emplace_back(temp_i);
		}
	}

	//�R���g���[���[���Ȃ����Ă��Ȃ���΃L�[�{�[�h����݂̂ɐ؂�ւ�
	if (Joystick::IsValid() == false) Input::SetKeyboardDevice();

	return true;
}

void InputConfig::DestroyConfig() {
	for (auto& map : input) {
		delete map.second;
	}
}