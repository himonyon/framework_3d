#include "../../framework.h"
#include "../../environment.h"


//コンストラクタ
Scene::Scene()
{
}

//デストラクタ
Scene::~Scene()
{
}

//処理
void Scene::Execute()
{
	//画像の処理
	GameObjectManager::Execute();
}

void Scene::Render()
{
	GameObjectManager::Render();
}


