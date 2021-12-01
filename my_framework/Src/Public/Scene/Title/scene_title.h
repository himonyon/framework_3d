#pragma once

#include "move.h"
#include "scale.h"

using spl = SpritePool;

class SceneTitle : public Scene {
private:
	noDel_ptr<Mesh> pTest_mesh;
	noDel_ptr<GameObject> pObj1;
	noDel_ptr<GameObject> pObj2;
	noDel_ptr<GameObject> pObj3;
	noDel_ptr<GameObject> pObj4;
	noDel_ptr<GameObject> pObj5;

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