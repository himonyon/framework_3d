/*-----------------------------------------------------------

	シーン管理クラス
		シーン全体を管理する
		シーン遷移や破棄を行う

-------------------------------------------------------------*/
#pragma once
//シーンクラス定義ファイル
#include "scene.h"
#include "Title/scene_title.h"
#include "Game/scene_game.h"

//シーンの種類
enum class eSceneTable
{
	None,
	Title,
	Game,
	SceneMax,
};

namespace MyFrameWork {
	class SceneManager {
	private:
		static Scene* pScene;
		static Scene* pReservedScene;

		static eSceneTable currentScene;
		static eSceneTable reservedScene;

	public:
		SceneManager();
		~SceneManager();

		//予約シーンの作成
		static void CreateReserveScene(eSceneTable scene);

		//シーンの切り替え予約
		static void SwitchScene(eSceneTable scene);
		//シーンの切り替え
		static void SwitchScene();

		//シーンの削除
		static void DeleteScene();
		static void DeleteMainScene();
		static void DeleteReserveScene();

		//Getter,Setter
		/// <summary>
		/// 引数の列挙のintと一致する値を持つシーンを返す
		/// </summary>
		static Scene*& GetScene(int scene);

		//描画対象シーン(pScene)を返す
		static Scene*& GetMainScene() { return pScene; }
	};
}