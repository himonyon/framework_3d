#pragma once

class SceneTitle : public Scene {
private:
	Sprite* sprite;
	Sprite* sprite2;
	Sound* sound0;

private:
	//������
	bool Initialize();

	//�폜
	void Terminate();

	//����
	void Execute();

	//�`��
	void Render();
};