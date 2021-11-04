#include "../../framework.h"
#include "../../environment.h"

//‰Šú‰»
bool SceneTitle::Initialize() {
	sprite = new Sprite(500, 300, 200, 200, L"Data/Image/sample.png");
	sprite->SetRenderPriority(2);
	sprite->vtx[0].a = 0.2f;
	sprite2 = new Sprite(100, 300, 200, 200, L"Data/Image/sample.png");
	sound0 = new Sound(L"Data/Sound/title_bgm.wav");
	sound0->Play();

	model = new Model(L"Data/Object/cube.obj");


	return true;
}

void SceneTitle::Terminate() {
	delete model;
}

//ˆ—
void SceneTitle::Execute() {
	if (Input::KeyTrg(DIK_SPACE)) {
		switchScene(eSceneTable::Game);
	}

	if (Input::GetJoyLX()) {
		sprite2->posX += Input::GetJoyLX() * 2;
	}

	if (Input::JoyButtonTrg(JOY_SQUARE)) {
		sprite->isRenderEnable() ? sprite->SetRenderEnable(false) : sprite->SetRenderEnable(true);
	}

	if (Input::JoyButtonTrg(JOY_CIRCLE)) {
		sprite->SetRenderPriority(-1);
	}
	if (Input::JoyButtonTrg(JOY_TRIANGLAR)) {
		sprite->SetRenderPriority(2);
	}

	Font::SetRect(200, 100, 500, WINDOW_HEIGHT);
	Font::SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	Font::SetColor(0xffffffff);
	Font::Print(L"testes‚È‚È‚Ètwstsse@%d ", Input::JoyPovOn(18000));
	Font::SetColor(0xff88ff88);
	Font::Print(200, 130, L"vvv@%d ", Input::JoyPovOn(18000));
	Font::Print(200, 160, L"vvv@%d ", Input::JoyPovOn(18000));
	Font::SetRect();


	Scene::Execute();
}

//•`‰æ
void SceneTitle::Render() {
	Scene::Render();
	model->Render();
}
