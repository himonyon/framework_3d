#include "../../framework.h"
#include "../../environment.h"

//������
bool SceneGame::Initialize() {
	sprite = new Sprite(10, 10, 200, 200, L"Data/Image/sample.png");
	sprite->vtx[0].a = 0.2f;
	sound0 = new Sound(L"Data/Sound/title_bgm.wav");
	sound0->Play();

	return true;
}

void SceneGame::Terminate() {

}

//����
void SceneGame::Execute() {
	if (Input::MouseTrg(1)) {
		switchScene(eSceneTable::Title);
	}


	Scene::Execute();
}

//�`��
void SceneGame::Render() {
	Scene::Render();
}
