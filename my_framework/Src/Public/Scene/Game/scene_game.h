#pragma once

class SceneGame : public Scene{
private:
	noDel_ptr<GameObject> pSample0;
	noDel_ptr<Sound> pSound0;
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