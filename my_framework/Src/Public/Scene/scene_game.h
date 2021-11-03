#pragma once

class SceneGame : public Scene{
private:
	Sprite* sprite;
	Sound* sound0;
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