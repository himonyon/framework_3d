#include "../../framework.h"
#include "../../environment.h"

Scene* SceneManager::pScene = NULL;
Scene* SceneManager::pReservedScene = NULL;
eSceneTable SceneManager::currentScene = eSceneTable::None;
eSceneTable SceneManager::reservedScene = eSceneTable::Title; //初期シーン

SceneManager::SceneManager() {
}
SceneManager::~SceneManager() {
}

void SceneManager::SwitchScene(eSceneTable scene) {
	reservedScene = scene;
}
//シーンの切り替え
void SceneManager::SwitchScene() {
	//切り替え予約のチェック
	if (currentScene == reservedScene)
	{
		return;
	}

	if (pReservedScene == nullptr) {
		//シーンの作成
		CreateReserveScene(reservedScene);
	}
	else {
		if (pReservedScene->isInitialized) {
			//遷移予定のシーンの種類(reservedScene)がすでに作成している予約シーンの種類と違えば処理しない
			if (pReservedScene->GetSceneType() != (int)reservedScene) return;

			//シーン削除
			DeleteMainScene();

			//現在のシーンを設定
			currentScene = reservedScene;

			//メインのシーンに予約シーンを渡す
			pScene = pReservedScene;

			//予約シーンはNullにする
			pReservedScene = nullptr;
		}
	}
}

//予約シーンの作成
void SceneManager::CreateReserveScene(eSceneTable scene) {
	if (pReservedScene != nullptr)	return;

	//シーンの作成
	switch (scene)
	{
	case eSceneTable::Title: pReservedScene = new SceneTitle(); break;
	case eSceneTable::Game:	pReservedScene = new SceneGame(); break;
	}

	//新たに作成したシーンのオブジェクトマネージャーにシーンの種類を渡す
	pReservedScene->SetSceneType((int)scene);

	//シーンの初期化
	std::thread th(&Scene::Initialize, pReservedScene);

	th.detach();
}

//シーンの削除
void SceneManager::DeleteScene() {
	if (pScene != NULL)
	{
		//シーンの終了
		pScene->Terminate();
		delete pScene;
		pScene = NULL;
	}
	if (pReservedScene != NULL)
	{
		//シーンの終了
		pReservedScene->Terminate();
		delete pReservedScene;
		pReservedScene = NULL;
	}
}
//シーンの削除
void SceneManager::DeleteMainScene() {
	if (pScene != NULL)
	{
		//シーンの終了
		pScene->Terminate();
		delete pScene;
		pScene = NULL;
	}
}
//シーンの削除
void SceneManager::DeleteReserveScene() {
	if (pReservedScene != NULL)
	{
		//シーンの終了
		pReservedScene->Terminate();
		delete pReservedScene;
		pReservedScene = NULL;
	}
}

//シーンの取得
Scene*& SceneManager::GetScene(int scene) {
	if (pScene != NULL && pScene->GetSceneType() == scene) return pScene;
	else if(pReservedScene != NULL && pReservedScene->GetSceneType() == scene) return pReservedScene;
	return pScene;
}