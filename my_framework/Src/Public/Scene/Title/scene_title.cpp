#include "../../../../framework.h"
#include "../../../../environment.h"

//初期化
void SceneTitle::Initialize() {
	//２Dオブジェクト
	pTest_sp = CreateSprite(new Sprite(L"Data/Image/bg.spr"));

	pCam = CreateObject(0, 0, -40.0f);
	pCam->AddComponent<Camera>();
	pCam->GetComponent<Camera>()->SetMain();

	pObj1 = CreateImageObject(0, 0, 100, 100, pTest_sp);
	pObj3[0] = CreateObject(0, 0, 0, CreateModel("Data/Object/parent0.fbx"));
	pObj5 = CreateObject(1, 0,0,pTest_sp);

	//ビヘイビア
	pObj1->AddComponent<Move>();
	pObj1->AddComponent<Scale>();

	pText = CreateObject(500, 50, 0, nullptr, "text");
	pText->AddComponent<Font>();
	pText->GetComponent<Font>()->Print(500, 50, L"%d %d", Mouse::GetX(), Mouse::GetY());

	//サウンド
	pSound0 = std::make_unique<Sound>(L"Data/Sound/title_bgm.wav");
	pSound0->SetVolume(0.1f);
	pSound0->Play();

	isInitialized = true;
}

void SceneTitle::Terminate() {
}

//処理
void SceneTitle::Execute() {
	int aa = 99;

	if (Input::On(InputConfig::input["decide"])) {
		pObj3[0]->transform->rotation.x += 1;
	}
	if (Input::On(InputConfig::input["cancel"])) {
		pCam->transform->rotation.y -= 1;
	}

	if (Input::On(InputConfig::input["up"])) {
		pCam->transform->position.z += 0.1f;
	}
	if (Input::On(InputConfig::input["down"])) {
 		pCam->transform->position.z -= 0.1f;
	}
	if (Input::Trg(InputConfig::input["right"])) {
		pCam->transform->position.x += 1;
	}
	if (Input::On(InputConfig::input["left"])) {
		pCam->transform->position.x -= 0.1f;
	} 

	if (Keyboard::On(DIK_F)) {
		pCam->transform->scale.x += 0.2f;
		pCam->transform->scale.y += 0.2f;
		pCam->transform->scale.z += 0.2f;
	}
	if (Keyboard::On(DIK_G)) {
		pCam->transform->scale.x -= 0.2f;
		pCam->transform->scale.y -= 0.2f;
		pCam->transform->scale.z -= 0.2f;
	}

	if (Keyboard::On(DIK_W)) {
		pCam->transform->position.y += 0.1f;
	}
	if (Keyboard::On(DIK_S)) {
		pCam->transform->position.y -= 0.1f;
	}

	Scene::Execute();
}

//描画
void SceneTitle::Render() {
	Scene::Render();
}
