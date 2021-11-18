#pragma once

//�V�[���̎��
enum class eSceneTable
{
	Title,
	Game,
	SceneMax,
};

//�V�[���N���X��`�t�@�C��
#include "scene.h"
#include "Title/scene_title.h"
#include "Game/scene_game.h"



//�V�[���؂�ւ��֐�(�\��)
void switchScene(eSceneTable scene);
//�V�[���؂�ւ��֐�(������)
void switchScene();
//�V�[���폜
void deleteScene();
