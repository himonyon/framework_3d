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
#include "scene_title.h"
#include "scene_game.h"



//�V�[���؂�ւ��֐�(�\��)
void switchScene(eSceneTable scene);
//�V�[���؂�ւ��֐�(������)
void switchScene();
//�V�[���폜
void deleteScene();
