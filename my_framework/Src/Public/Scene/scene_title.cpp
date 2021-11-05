#include "../../framework.h"
#include "../../environment.h"

//初期化
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

//処理
void SceneTitle::Execute() {
	if (Input::KeyTrg(DIK_SPACE)) {
		switchScene(eSceneTable::Game);
	}

	////
	///移動・回転・スケーリング
	////
	// 
	//回転
	if (Input::KeyOn(DIK_R)) {
		if (Input::KeyOn(DIK_LEFT)) {
			model->rotX -= 0.1f;
		}
		if (Input::KeyOn(DIK_RIGHT)) {
			model->rotX += 0.1f;
		}
		if (Input::KeyOn(DIK_LSHIFT)) {
			if (Input::KeyOn(DIK_DOWN)) {
				model->rotY += 0.1f;
			}
			if (Input::KeyOn(DIK_UP)) {
				model->rotY -= 0.1f;
			}
		}
		else {
			if (Input::KeyOn(DIK_DOWN)) {
				model->rotZ += 0.1f;
			}
			if (Input::KeyOn(DIK_UP)) {
				model->rotZ -= 0.1f;
			}
		}
	}
	//スケーリング
	else if (Input::KeyOn(DIK_S)) {
		if (Input::KeyOn(DIK_LEFT)) {
			model->scaleX -= 0.1f;
		}
		if (Input::KeyOn(DIK_RIGHT)) {
			model->scaleX += 0.1f;
		}
		if (Input::KeyOn(DIK_LSHIFT)) {
			if (Input::KeyOn(DIK_DOWN)) {
				model->scaleY += 0.1f;
			}
			if (Input::KeyOn(DIK_UP)) {
				model->scaleY -= 0.1f;
			}
		}
		else {
			if (Input::KeyOn(DIK_DOWN)) {
				model->scaleZ += 0.1f;
			}
			if (Input::KeyOn(DIK_UP)) {
				model->scaleZ -= 0.1f;
			}
		}
	}
	else {
		//移動
		if (Input::KeyOn(DIK_LEFT)) {
			model->posX -= 0.1f;
		}
		if (Input::KeyOn(DIK_RIGHT)) {
			model->posX += 0.1f;
		}
		if (Input::KeyOn(DIK_LSHIFT)) {
			if (Input::KeyOn(DIK_UP)) {
				model->posY += 0.1f;
			}
			if (Input::KeyOn(DIK_DOWN)) {
				model->posY -= 0.1f;
			}
		}
		else {
			if (Input::KeyOn(DIK_UP)) {
				model->posZ += 0.1f;
			}
			if (Input::KeyOn(DIK_DOWN)) {
				model->posZ -= 0.1f;
			}
		}
	}


	Font::SetRect(200, 100, 500, WINDOW_HEIGHT);
	Font::SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	Font::SetColor(0xffffffff);
	Font::Print(L"testesなななtwstsse　%d ", Input::JoyPovOn(18000));
	Font::SetColor(0xff88ff88);
	Font::Print(200, 130, L"vvv　%d ", Input::JoyPovOn(18000));
	Font::Print(200, 160, L"vvv　%d ", Input::JoyPovOn(18000));
	Font::SetRect();


	Scene::Execute();
}

//描画
void SceneTitle::Render() {
	Scene::Render();
	model->Render();
}
