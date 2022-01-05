#include "../../../../framework.h"
#include "../../../../environment.h"

//初期化
void SceneTitle::Initialize() {
	//２Dオブジェクト
	pTest_mesh = CreateMesh(L"Data/Object/test.obj");
	pTest_sp = CreateSprite(new Sprite(L"Data/Image/bg.spr"));

	pCam = CreateObject(0, 0, -15.0f);
	pCam->AddComponent<Camera>();
	pCam->GetComponent<Camera>()->SetMain();

	pObj1 = CreateImageObject(0, 0, 100, 100, pTest_sp);
	pObj2 = CreateObject(0, -2,0, 5, 5, pTest_sp);
	pObj4 = CreateObject(0, 0, 0, CreateMesh(L"Data/Object/Chips.obj"));

	pObj5 = CreateImageObject(1000, 500, 50, 50, CreateSprite(new Sprite(L"Data/Image/Chips_Cover.spr")));
	pObj5->AddComponent<Collider2D>();
	pObj5->GetComponent<Collider2D>()->SetUpCollider2D(true);

	//コライダーテスト
	pObj2->AddComponent<Collider2D>();
	pObj2->GetComponent<Collider2D>()->SetUpCollider2D(true);

	//アニメーションテスト
	pObj2->AddComponent<Animator>();
	noDel_ptr<Animator> an = pObj2->GetComponent<Animator>();
	an->AddAnimation("Test", new SpriteAnimation(L"Data/Animation/test.anim", true));
	an->PlayAnim("Test");

	//ビヘイビア
	pObj1->AddComponent<Move>();
	pObj1->AddComponent<Scale>();
	pObj1->transform->position.z = 10;

	for (int i = 0; i < 5; i++) {
		pObj3[i] = CreateObject(-3 + (1*i), -1, 0, 1, 1, pTest_sp);
	}

	pText = CreateObject(500, 50, 0, nullptr, "text");;
	pText->AddComponent<Font>();
	pText->GetComponent<Font>()->Print(500, 50, L"%d %d", Mouse::GetX(), Mouse::GetY());

	//サウンド
	pSound0 = std::make_unique<Sound>(L"Data/Sound/title_bgm.wav");
	pSound0->SetVolume(0.2f);
	pSound0->Play();

	isInitialized = true;
}

void SceneTitle::Terminate() {
}

//処理
void SceneTitle::Execute() {
	int aa = 99;

	if (Input::On(InputConfig::input["decide"])) {
		pCam->transform->scale.x += 0.1f;
	}
	if (Input::On(InputConfig::input["cancel"])) {
		pCam->transform->scale.x -= 0.1f;
	}

	if (Input::On(InputConfig::input["up"])) {
		pCam->transform->position.z += 0.1f;
	}
	if (Input::On(InputConfig::input["down"])) {
		pCam->transform->position.z -= 0.1f;
	}
	if (Input::On(InputConfig::input["right"])) {
		pCam->transform->position.x += 0.1f;
	}
	if (Input::On(InputConfig::input["left"])) {
		pCam->transform->position.x -= 0.1f;
	} 

	if (Keyboard::On(DIK_A)) {
		pCam->transform->rotation.x += 0.1f;
	}
	if (Keyboard::On(DIK_D)) {
		pCam->transform->rotation.x -= 0.1f;
	}

	Scene::Execute();
}

//描画
void SceneTitle::Render() {
	Scene::Render();
}
