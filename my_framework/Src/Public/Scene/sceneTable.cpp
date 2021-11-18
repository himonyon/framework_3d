
#include "../../framework.h"
#include "../../environment.h"

//起動時のシーン
eSceneTable initialScene = eSceneTable::Title;
//現在のシーン
eSceneTable currentScene = (eSceneTable)-1;
//切り替え予約のシーン
eSceneTable reserveScene = initialScene;
//シーンのインスタンス
Scene* pScene = NULL;


//シーン切り替え関数(予約)
void switchScene(eSceneTable scene)
{
	reserveScene = scene;
}

//シーン切り替え関数(実処理)
void switchScene()
{
	//切り替え予約のチェック
	if (currentScene == reserveScene)
	{
		return;
	}

	//シーン削除
	deleteScene();

	//シーンの作成
	switch (reserveScene)
	{
	case eSceneTable::Title:		pScene = new SceneTitle();			break;
	case eSceneTable::Game:			pScene = new SceneGame();			break;
	}

	//現在のシーンを設定
	currentScene = reserveScene;

	//シーンの初期化
	pScene->Initialize();
}

//シーン削除
void deleteScene()
{
	if (pScene != NULL)
	{
		//シーンの終了
		delete pScene;
		pScene = NULL;
	}
}
