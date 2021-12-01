#include "../../framework.h"
#include "../../environment.h"

//������
bool SceneGame::Initialize() {
	pSample0 = CreateObject(0,0,0);

	pSound0 = noDel_ptr<Sound>(CreateSound(L"Data/Sound/title_bgm.wav"));
	pSound0->Play();

	return true;
}

void SceneGame::Terminate() {

}

//����
void SceneGame::Execute() {
	if (Input::Trg(InputConfig::cancel)) {
		SceneManager::SwitchScene(eSceneTable::Title);
	}

	Scene::Execute();
}

//�`��
void SceneGame::Render() {
	Scene::Render();
}
