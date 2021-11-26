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
	static std::vector<int> decide;
	static std::vector<int> cancel;
	static std::vector<int> cursorUp;
	static std::vector<int> cursorDown;
	static std::vector<int> cursorRight;
	static std::vector<int> cursorLeft;
	static std::vector<int> moveX;
	static std::vector<int> moveY;

	static bool SetUpConfig();
};