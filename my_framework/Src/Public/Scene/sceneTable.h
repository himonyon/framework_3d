#pragma once

//シーンの種類
enum class eSceneTable
{
	Title,
	Game,
	SceneMax,
};

//シーンクラス定義ファイル
#include "scene.h"
#include "Title/scene_title.h"
#include "Game/scene_game.h"



//シーン切り替え関数(予約)
void switchScene(eSceneTable scene);
//シーン切り替え関数(実処理)
void switchScene();
//シーン削除
void deleteScene();
