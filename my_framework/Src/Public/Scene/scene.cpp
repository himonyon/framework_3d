#include "../../framework.h"
#include "../../environment.h"


//�R���X�g���N�^
Scene::Scene()
{
}

//�f�X�g���N�^
Scene::~Scene()
{
}

//����
void Scene::Execute()
{
	//�摜�̏���
	GameObjectManager::Execute();
}

void Scene::Render()
{
	GameObjectManager::Render();
}


//�͈͓��Ő����𑝌�������
int Scene::CountUpInRange(int target, int maxRange) {
	if (target + 1 > maxRange) return 0;
	return ++target;
}
int Scene::CountDownInRange(int target, int maxRange) {
	if (target - 1 < 0) return maxRange;
	return --target;
}