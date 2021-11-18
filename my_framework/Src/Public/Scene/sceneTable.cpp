
#include "../../framework.h"
#include "../../environment.h"

//�N�����̃V�[��
eSceneTable initialScene = eSceneTable::Title;
//���݂̃V�[��
eSceneTable currentScene = (eSceneTable)-1;
//�؂�ւ��\��̃V�[��
eSceneTable reserveScene = initialScene;
//�V�[���̃C���X�^���X
Scene* pScene = NULL;


//�V�[���؂�ւ��֐�(�\��)
void switchScene(eSceneTable scene)
{
	reserveScene = scene;
}

//�V�[���؂�ւ��֐�(������)
void switchScene()
{
	//�؂�ւ��\��̃`�F�b�N
	if (currentScene == reserveScene)
	{
		return;
	}

	//�V�[���폜
	deleteScene();

	//�V�[���̍쐬
	switch (reserveScene)
	{
	case eSceneTable::Title:		pScene = new SceneTitle();			break;
	case eSceneTable::Game:			pScene = new SceneGame();			break;
	}

	//���݂̃V�[����ݒ�
	currentScene = reserveScene;

	//�V�[���̏�����
	pScene->Initialize();
}

//�V�[���폜
void deleteScene()
{
	if (pScene != NULL)
	{
		//�V�[���̏I��
		delete pScene;
		pScene = NULL;
	}
}
