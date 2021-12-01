/*-----------------------------------------------------------

	タイマークラス
		個別にタイマーを使いたいときに利用

-------------------------------------------------------------*/
#pragma once

class Timer {
private:
	static float frameTime; //フレームタイム
	static float beforeTime; //前のフレームの時間

	bool flag = false; //タイマーフラグ

public:
	float time = 0;

public:
	static void FrameTimeExecute();

	void Start();
	void Stop();
	void Resume();
	void Execute();

	//getter
	float GetFrameTime() const { return frameTime; }
};