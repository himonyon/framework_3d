#include "../../framework.h"
#include "../../environment.h"

/// <summary>
/// ユーザー設定インプット情報の初期化
/// </summary>
std::vector<int> InputConfig::decide = {};
std::vector<int> InputConfig::cancel = {};
std::vector<int> InputConfig::cursorUp = {};
std::vector<int> InputConfig::cursorDown = {};
std::vector<int> InputConfig::cursorRight = {};
std::vector<int> InputConfig::cursorLeft = {};
std::vector<int> InputConfig::moveX = {};
std::vector<int> InputConfig::moveY = {};

bool InputConfig::SetUpConfig() {
	char _key[256] = { 0 };
	//ファイルを開いて内容を読み込む
	FILE* fp = NULL;
	_wfopen_s(&fp, L"Data/Input/InputConfig.txt", L"rt");
	if (fp == NULL) return false;

	std::vector<int>* temp_vec = &decide;

	//まずは頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));
		//ボタン名
		if (strcmp(_key, "decide") == 0) temp_vec = &decide;
		else if (strcmp(_key, "cancel") == 0) temp_vec = &cancel;
		else if (strcmp(_key, "cursorUp") == 0) temp_vec = &cursorUp;
		else if (strcmp(_key, "cursorDown") == 0) temp_vec = &cursorDown;
		else if (strcmp(_key, "cursorRight") == 0) temp_vec = &cursorRight;
		else if (strcmp(_key, "cursorLeft") == 0) temp_vec = &cursorLeft;
		else if (strcmp(_key, "moveX") == 0) temp_vec = &moveX;
		else if (strcmp(_key, "moveY") == 0) temp_vec = &moveY;
		//各コンテナに数字を格納する
		else if (strcmp(_key, "") != 0){
			int temp_i = std::stoi(_key);
			temp_vec->emplace_back(temp_i);
		}
	}

	return true;
}