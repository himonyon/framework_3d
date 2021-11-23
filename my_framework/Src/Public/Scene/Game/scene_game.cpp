#include "../../framework.h"
#include "../../environment.h"

//‰Šú‰»
bool SceneGame::Initialize() {
	pSample0 = noDel_ptr<GameObject2D>(CreateObject2D<GameObject2D>(
		new GameObject2D(10, 10, 200, 200, 
			CreateSprite(L"Data/Image/sample.png",0.5f))));

	pSound0 = noDel_ptr<Sound>(CreateSound(L"Data/Sound/title_bgm.wav"));
	pSound0->Play();

	return true;
}

void SceneGame::Terminate() {

}

//ˆ—
void SceneGame::Execute() {
	if (Input::Trg(InputConfig::cancel)) {
		SceneManager::SwitchScene(eSceneTable::Title);
	}

	Scene::Execute();
}

//•`‰æ
void SceneGame::Render() {
	Scene::Render();
}
