#include "../../framework.h"
#include "../../environment.h"

Scene* SceneManager::pScene = NULL;
Scene* SceneManager::pReservedScene = NULL;
eSceneTable SceneManager::currentScene = eSceneTable::None;
eSceneTable SceneManager::reservedScene = eSceneTable::Title; //�����V�[��

SceneManager::SceneManager() {
}
SceneManager::~SceneManager() {
}

void SceneManager::SwitchScene(eSceneTable scene) {
	reservedScene = scene;
}
//�V�[���̐؂�ւ�
void SceneManager::SwitchScene() {
	//�؂�ւ��\��̃`�F�b�N
	if (currentScene == reservedScene)
	{
		return;
	}

	if (pReservedScene == nullptr) {
		//�V�[���̍쐬
		CreateReserveScene(reservedScene);
	}
	else {
		if (pReservedScene->isInitialized) {
			//�J�ڗ\��̃V�[���̎��(reservedScene)�����łɍ쐬���Ă���\��V�[���̎�ނƈႦ�Ώ������Ȃ�
			if (pReservedScene->GetSceneType() != (int)reservedScene) return;

			//�V�[���폜
			DeleteMainScene();

			//���݂̃V�[����ݒ�
			currentScene = reservedScene;

			//���C���̃V�[���ɗ\��V�[����n��
			pScene = pReservedScene;

			//�\��V�[����Null�ɂ���
			pReservedScene = nullptr;
		}
	}
}

//�\��V�[���̍쐬
void SceneManager::CreateReserveScene(eSceneTable scene) {
	if (pReservedScene != nullptr)	return;

	//�V�[���̍쐬
	switch (scene)
	{
	case eSceneTable::Title: pReservedScene = new SceneTitle(); break;
	case eSceneTable::Game:	pReservedScene = new SceneGame(); break;
	}

	//�V���ɍ쐬�����V�[���̃I�u�W�F�N�g�}�l�[�W���[�ɃV�[���̎�ނ�n��
	pReservedScene->SetSceneType((int)scene);

	//�V�[���̏�����
	std::thread th(&Scene::Initialize, pReservedScene);

	th.detach();
}

//�V�[���̍폜
void SceneManager::DeleteScene() {
	if (pScene != NULL)
	{
		//�V�[���̏I��
		pScene->Terminate();
		delete pScene;
		pScene = NULL;
	}
	if (pReservedScene != NULL)
	{
		//�V�[���̏I��
		pReservedScene->Terminate();
		delete pReservedScene;
		pReservedScene = NULL;
	}
}
//�V�[���̍폜
void SceneManager::DeleteMainScene() {
	if (pScene != NULL)
	{
		//�V�[���̏I��
		pScene->Terminate();
		delete pScene;
		pScene = NULL;
	}
}
//�V�[���̍폜
void SceneManager::DeleteReserveScene() {
	if (pReservedScene != NULL)
	{
		//�V�[���̏I��
		pReservedScene->Terminate();
		delete pReservedScene;
		pReservedScene = NULL;
	}
}

//�V�[���̎擾
Scene*& SceneManager::GetScene(int scene) {
	if (pScene != NULL && pScene->GetSceneType() == scene) return pScene;
	else if(pReservedScene != NULL && pReservedScene->GetSceneType() == scene) return pReservedScene;
	return pScene;
}