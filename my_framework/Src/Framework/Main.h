#pragma once
#include "../../framework.h"
#include "../../environment.h"


//�N�����̃V�[��
extern eSceneTable initialScene;
//�V�[���̃C���X�^���X
extern Scene* pScene;

class Main {
public:
	bool Init(void* hWnd);
	void Destroy();
	void App();
	void Execute();
	void Render();
};