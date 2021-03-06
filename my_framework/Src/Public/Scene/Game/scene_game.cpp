#include "../../framework.h"
#include "../../environment.h"

//初期化
void SceneGame::Initialize() {
	pSample0 = CreateImageObject(500, 500, 50, 50, CreateSprite(new Sprite(L"Data/Image/Chips_Cover.spr")));

	pSound0 = std::make_unique<Sound>(L"Data/Sound/title_bgm.wav");
	pSound0->Play();

	isInitialized = true;
}

void SceneGame::Terminate() {

}

//処理
void SceneGame::Execute() {
	if (Input::Trg(InputConfig::input["decide"])) {
		SceneManager::SwitchScene(eSceneTable::Title);
	}

	Scene::Execute();
}

//描画
void SceneGame::Render() {
	Scene::Render();
}
