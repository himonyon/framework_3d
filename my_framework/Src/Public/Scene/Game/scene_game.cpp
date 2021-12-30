#include "../../framework.h"
#include "../../environment.h"

//‰Šú‰»
void SceneGame::Initialize() {
	pSample0 = CreateObject(500, 500, 50, 50, CreateSprite(new Sprite(L"Data/Image/Chips_Cover.spr")));

	pSound0 = std::make_unique<Sound>(L"Data/Sound/title_bgm.wav");
	pSound0->Play();

	isInitialized = true;
}

void SceneGame::Terminate() {

}

//ˆ—
void SceneGame::Execute() {
	if (Input::Trg(InputConfig::input["decide"])) {
		SceneManager::SwitchScene(eSceneTable::Title);
	}

	Scene::Execute();
}

//•`‰æ
void SceneGame::Render() {
	Scene::Render();
}
