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


