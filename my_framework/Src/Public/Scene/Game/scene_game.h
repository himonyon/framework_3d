#pragma once

class SceneGame : public Scene{
private:
	noDel_ptr<GameObject> pSample0;
	std::unique_ptr<Sound> pSound0;
private:
	//初期化
	void Initialize();

	//削除
	void Terminate();

	//処理
	void Execute();

	//描画
	void Render();
};