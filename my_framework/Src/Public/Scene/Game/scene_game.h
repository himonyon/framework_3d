#pragma once

class SceneGame : public Scene{
private:
	noDel_ptr<GameObject> pSample0;
	std::unique_ptr<Sound> pSound0;
private:
	//‰Šú‰»
	void Initialize();

	//íœ
	void Terminate();

	//ˆ—
	void Execute();

	//•`‰æ
	void Render();
};