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


//範囲内で数字を増減させる
int Scene::CountUpInRange(int target, int maxRange) {
	if (target + 1 > maxRange) return 0;
	return ++target;
}
int Scene::CountDownInRange(int target, int maxRange) {
	if (target - 1 < 0) return maxRange;
	return --target;
}