#pragma once
#include "../../framework.h"
#include "../../environment.h"


//起動時のシーン
extern eSceneTable initialScene;
//シーンのインスタンス
extern Scene* pScene;

class Main {
public:
	bool Init(void* hWnd);
	void Destroy();
	void App();
	void Execute();
	void Render();
};