#pragma once

class SceneGame : public Scene{
private:
	noDel_ptr<GameObject> pSample0;
	std::unique_ptr<Sound> pSound0;
private:
	//������
	void Initialize();

	//�폜
	void Terminate();

	//����
	void Execute();

	//�`��
	void Render();
};