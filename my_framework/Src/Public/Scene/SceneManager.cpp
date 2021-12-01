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

	//シーン削除
	DeleteMainScene();

	//シーンの作成
	switch (reservedScene)
	{
	case eSceneTable::Title: pScene = new SceneTitle(); break;
	case eSceneTable::Game:	pScene = new SceneGame(); break;
	}

	//現在のシーンを設定
	currentScene = reservedScene;

	//新たに作成したシーンのオブジェクトマネージャーにシーンの種類を渡す
	pScene->SetSceneType((int)currentScene);


	//シーンの初期化
	pScene->Initialize();
}

//シーンの削除
void SceneManager::DeleteScene() {
	if (pScene != NULL)
	{
		//シーンの終了
		delete pScene;
		pScene = NULL;
	}
	if (pReservedScene != NULL)
	{
		//シーンの終了
		delete pReservedScene;
		pReservedScene = NULL;
	}
}
//シーンの削除
void SceneManager::DeleteMainScene() {
	if (pScene != NULL)
	{
		//シーンの終了
		delete pScene;
		pScene = NULL;
	}
}
//シーンの削除
void SceneManager::DeleteReserveScene() {
	if (pReservedScene != NULL)
	{
		//シーンの終了
		delete pReservedScene;
		pReservedScene = NULL;
	}
}

//シーンの取得
Scene*& SceneManager::GetScene(int scene) {
	return pScene;
}