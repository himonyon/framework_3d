#pragma once

class SceneGame : public Scene{
private:
	Sprite* sprite;
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