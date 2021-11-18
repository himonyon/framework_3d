#pragma once

/// <summary>
/// キーボード、ジョイスティックのユーザー設定用クラス
/// キーボードとジョイスティックの各ボタンに共通の役割をもたせる
/// </summary>

class InputConfig {
private:
	static const int DeviceNum = 2;
public:
	static int Decide[DeviceNum];
	static int Cansel[DeviceNum];
};