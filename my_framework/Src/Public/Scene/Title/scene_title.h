#pragma once

#include "move.h"
#include "scale.h"

class SceneTitle : public Scene {
private:
	noDel_ptr<Mesh> pTest_mesh;
	noDel_ptr<Sprite> pTest_sp;

	noDel_ptr<GameObject> pCam;
	noDel_ptr<GameObject> pObj1;
	noDel_ptr<GameObject> pObj2;
	noDel_ptr<GameObject> pObj3[5];
	noDel_ptr<GameObject> pObj4;

	noDel_ptr<GameObject> pObj5;
	noDel_ptr<GameObject> pText;

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