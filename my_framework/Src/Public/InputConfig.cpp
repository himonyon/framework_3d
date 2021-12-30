#include "../../framework.h"
#include "../../environment.h"

std::unordered_map<std::string, std::vector<int>*> InputConfig::input;

bool InputConfig::SetUpConfig() {
	char _key[64] = { 0 };
	//ファイルを開いて内容を読み込む
	FILE* fp = NULL;
	_wfopen_s(&fp, L"Data/Input/InputConfig.txt", L"rt");
	if (fp == NULL) return false;

	std::vector<int>* temp_vec = nullptr;

	//まずは頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));
		//ボタン名
		if (strcmp(_key, "name") == 0) {
			temp_vec = new std::vector<int>;
			fscanf_s(fp, "%s", _key, (int)sizeof(_key));
			std::string str = std::string(_key);
			input[str] = temp_vec;
		}
		else if (strcmp(_key, "end") == 0) {
			temp_vec = nullptr;
		}
		//各コンテナに数字を格納する
		else {
			int temp_i = std::stoi(_key);
			temp_vec->emplace_back(temp_i);
		}
	}

	return true;
}

void InputConfig::DestroyConfig() {
	for (auto& map : input) {
		delete map.second;
	}
}