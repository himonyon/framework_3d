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

	//�V�[���폜
	DeleteMainScene();

	//�V�[���̍쐬
	switch (reservedScene)
	{
	case eSceneTable::Title: pScene = new SceneTitle(); break;
	case eSceneTable::Game:	pScene = new SceneGame(); break;
	}

	//���݂̃V�[����ݒ�
	currentScene = reservedScene;

	//�V���ɍ쐬�����V�[���̃I�u�W�F�N�g�}�l�[�W���[�ɃV�[���̎�ނ�n��
	pScene->SetSceneType((int)currentScene);


	//�V�[���̏�����
	pScene->Initialize();
}

//�V�[���̍폜
void SceneManager::DeleteScene() {
	if (pScene != NULL)
	{
		//�V�[���̏I��
		delete pScene;
		pScene = NULL;
	}
	if (pReservedScene != NULL)
	{
		//�V�[���̏I��
		delete pReservedScene;
		pReservedScene = NULL;
	}
}
//�V�[���̍폜
void SceneManager::DeleteMainScene() {
	if (pScene != NULL)
	{
		//�V�[���̏I��
		delete pScene;
		pScene = NULL;
	}
}
//�V�[���̍폜
void SceneManager::DeleteReserveScene() {
	if (pReservedScene != NULL)
	{
		//�V�[���̏I��
		delete pReservedScene;
		pReservedScene = NULL;
	}
}

//�V�[���̎擾
Scene*& SceneManager::GetScene(int scene) {
	return pScene;
}