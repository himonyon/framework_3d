#pragma once

class SceneTitle : public Scene {
private:
	noDel_ptr<Sprite> pSp0;
	noDel_ptr<GameObject2D> pObj1;
	noDel_ptr<Sound> pSound0;
	noDel_ptr<GameObject2D> pObj2;
	noDel_ptr<SpriteAnimation> pAnim;

	noDel_ptr<GameObject3D> pChips;
	noDel_ptr<Mesh> pTest;

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