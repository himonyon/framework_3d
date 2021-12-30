/*-----------------------------------------------------------

	入力コンフィグクラス
		入力するボタンを用途別にまとめるクラス
		テキストファイルからそれぞれの用途に対応するボタンを取得

-------------------------------------------------------------*/
#pragma once

/// <summary>
/// キーボード、ジョイスティックのユーザー設定用クラス
/// キーボードとジョイスティックの各ボタンに共通の役割をもたせる
/// </summary>

class InputConfig {
public:
	static std::unordered_map<std::string, std::vector<int>*> input;

	static bool SetUpConfig();
	static void DestroyConfig();
};