/*-----------------------------------------------------------

	�V�[���Ǘ��N���X
		�V�[���S�̂��Ǘ�����
		�V�[���J�ڂ�j�����s��

-------------------------------------------------------------*/
#pragma once
//�V�[���N���X��`�t�@�C��
#include "scene.h"
#include "Title/scene_title.h"
#include "Game/scene_game.h"

//�V�[���̎��
enum class eSceneTable
{
	None,
	Title,
	Game,
	SceneMax,
};

class SceneManager {
private:
	static Scene* pScene;
	static Scene* pReservedScene;

	static eSceneTable currentScene;
	static eSceneTable reservedScene;

public:
	SceneManager();
	~SceneManager();

	//�V�[���̗\��
	static void SwitchScene(eSceneTable scene);
	//�V�[���̐؂�ւ�
	static void SwitchScene();

	//�V�[���̍폜
	static void DeleteScene();
	static void DeleteMainScene();
	static void DeleteReserveScene();

	//Getter,Setter
	
	/// <summary>
	/// �����̗񋓂�int�ƈ�v����l(sceneType)�����V�[����Ԃ�
	/// </summary>
	static Scene*& GetScene(int scene);

	//�`��ΏۃV�[��(pScene)��Ԃ�
	static Scene*& GetMainScene() { return pScene; }
};