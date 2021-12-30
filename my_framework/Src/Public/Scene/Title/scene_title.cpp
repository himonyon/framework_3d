#include "../../../../framework.h"
#include "../../../../environment.h"

//������
void SceneTitle::Initialize() {
	//�QD�I�u�W�F�N�g
	pTest_mesh = CreateMesh(L"Data/Object/test.obj");
	pTest_sp = CreateSprite(new Sprite(L"Data/Image/bg.spr"));

	pObj2 = CreateObject(480, 480, 300, 300, pTest_sp);
	pObj1 = CreateObject(480, 480, 100, 100, pTest_sp);
	pObj4 = CreateObject(100, 200, 50, 50, pTest_sp);

	pObj5 = CreateObject(1000, 500, 50, 50, CreateSprite(new Sprite(L"Data/Image/Chips_Cover.spr")));
	pObj5->AddComponent<Collider2D>();
	pObj5->GetComponent<Collider2D>()->SetUpCollider2D(true);

	//�R���C�_�[�e�X�g
	pObj2->AddComponent<Collider2D>();
	pObj2->GetComponent<Collider2D>()->SetUpCollider2D(true);

	//�A�j���[�V�����e�X�g
	pObj2->AddComponent<Animator>();
	noDel_ptr<Animator> an = pObj2->GetComponent<Animator>();
	an->AddAnimation("Test", new SpriteAnimation(L"Data/Animation/test.anim", true));
	an->PlayAnim("Test");

	//�r�w�C�r�A
	pObj1->AddComponent<Move>();
	pObj1->AddComponent<Scale>();
	pObj1->transform->position.z = 10;

	pObj3 = CreateObject(100, 500, 60, 60, pTest_sp);

	pText = CreateObject(500, 50, 0, nullptr, "text");;
	pText->AddComponent<Font>();
	pText->GetComponent<Font>()->Print(500, 50, L"%d %d", Mouse::GetX(), Mouse::GetY());

	//�T�E���h
	pSound0 = std::make_unique<Sound>(L"Data/Sound/title_bgm.wav");
	pSound0->SetVolume(0.2f);
	pSound0->Play();

	isInitialized = true;
}

void SceneTitle::Terminate() {
}

//����
void SceneTitle::Execute() {
	int aa = 99;

	if (Input::Trg(InputConfig::input["decide"])) {
		SceneManager::SwitchScene(eSceneTable::Game);
	}
	if (Input::Trg(InputConfig::input["cancel"])) {
		SceneManager::CreateReserveScene(eSceneTable::Game);
	}

	if (Keyboard::Trg(DIK_A)) {
		pObj1->GetComponent<SpriteRenderer>()->SetRenderPriority(100);
	}
	if (Keyboard::Trg(DIK_C)) {
		pObj1->GetComponent<SpriteRenderer>()->SetRenderPriority(-100);
	}

	Scene::Execute();
}

//�`��
void SceneTitle::Render() {
	Scene::Render();
}
