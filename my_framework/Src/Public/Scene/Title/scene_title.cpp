#include "../../../../framework.h"
#include "../../../../environment.h"

//初期化
bool SceneTitle::Initialize() {
	//２Dオブジェクト
	pTest_mesh = CreateMesh(L"Data/Object/test.obj");
	pTest_sp = CreateSprite(new Sprite(L"Data/Image/bg.spr"));

	pObj2 = CreateObject(480, 480, 300, 300, pTest_sp);
	pObj1 = CreateObject(480, 480, 100, 100, pTest_sp);
	pObj4 = CreateObject(100, 200, 50, 50, pTest_sp);

	pObj5 = CreateObject(1000, 500, 50, 50, CreateSprite(new Sprite(L"Data/Image/Chips_Cover.spr")));
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

	pObj3 = CreateObject(100, 500, 60, 60, pTest_sp);
	pText = CreateObject(500, 50, 0);;
	pText->AddComponent<Font>();


	//サウンド
	pSound0 = CreateSound(L"Data/Sound/title_bgm.wav");
	pSound0->Play();

	return true;
}

void SceneTitle::Terminate() {
}

//処理
void SceneTitle::Execute() {
	int aa = 99;

	pText->GetComponent<Font>()->Print(500, 50, L"aloke %d", aa);


	if (Keyboard::Trg(DIK_SPACE)) {
		pObj1->GetComponent<SpriteRenderer>()->SetRenderPriority(100);
	}
	if (Keyboard::Trg(DIK_C)) {
		pObj1->GetComponent<SpriteRenderer>()->SetRenderPriority(-100);
	}

	Scene::Execute();
}

//描画
void SceneTitle::Render() {
	Scene::Render();
}
