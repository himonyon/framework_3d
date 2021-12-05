#include "../../../../framework.h"
#include "../../../../environment.h"

//������
bool SceneTitle::Initialize() {
	//�QD�I�u�W�F�N�g
	pTest_mesh = CreateMesh(L"Data/Object/test.obj");
	pTest_sp = CreateSprite(new Sprite(L"Data/Image/bg.spr"));

	pObj1 = CreateObject(480, 480, 100, 100, pTest_sp);
	pObj2 = CreateObject(480, 480, 300, 300, pTest_sp);
	pObj4 = CreateObject(100, 200, 50, 50, pTest_sp);

	for (int i = 0; i < 500; i++) {
		pObj5[i] = CreateObject(-200, 500, 50, 50, CreateSprite(new Sprite(L"Data/Image/grid.spr")));
		pObj5[i]->AddComponent<Collider2D>();
		pObj5[i]->GetComponent<Collider2D>()->SetUpCollider2D(true);
	}

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

	pObj3 = CreateObject(100, 500, 60, 60, pTest_sp);

	//�T�E���h
	pSound0 = CreateSound(L"Data/Sound/title_bgm.wav");
	pSound0->Play();

	return true;
}

void SceneTitle::Terminate() {
}

//����
void SceneTitle::Execute() {
	if (Keyboard::Trg(DIK_SPACE)) {
		pObj1->GetComponent<SpriteRenderer>()->vtx[1].u = 1;
		pObj1->GetComponent<SpriteRenderer>()->vtx[3].u = 1;
	}

	Scene::Execute();
}

//�`��
void SceneTitle::Render() {
	Scene::Render();
}
