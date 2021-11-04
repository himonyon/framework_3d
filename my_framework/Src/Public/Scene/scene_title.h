#pragma once

class SceneTitle : public Scene {
private:
	Sprite* sprite;
	Sprite* sprite2;
	Sound* sound0;

	Model* model;

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