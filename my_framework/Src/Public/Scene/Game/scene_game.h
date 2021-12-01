#pragma once

class SceneGame : public Scene{
private:
	noDel_ptr<GameObject> pSample0;
	noDel_ptr<Sound> pSound0;
private:
	//‰Šú‰»
	bool Initialize();

	//íœ
	void Terminate();

	//ˆ—
	void Execute();

	//•`‰æ
	void Render();
};