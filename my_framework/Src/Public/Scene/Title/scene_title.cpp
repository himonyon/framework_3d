#include "../../../../framework.h"
#include "../../../../environment.h"

//������
bool SceneTitle::Initialize() {
	//�QD�I�u�W�F�N�g
	pTest_mesh = CreateMesh(L"Data/Object/test.obj");

	pObj1 = CreateObject(480, 480, 100, 100, spl::pSp0);
	pObj2 = CreateObject(480, 480, 100, 100, spl::pSp1);
	pObj3 = CreateObject(100, 500, 60, 60, spl::pSp1);
	pObj4 = CreateObject(100, 200, 50, 50, spl::pSp0);
	pObj5 = CreateObject(700, 500, 50, 50, spl::pSp0);

	//�R���C�_�[�e�X�g
	pObj2->AddComponent<Collider2D>(new Collider2D(true));

	//�A�j���[�V�����e�X�g
	pObj2->AddComponent<Animator>(new Animator());
	noDel_ptr<Animator> an = pObj2->GetComponent<Animator>();
	an->AddAnimation("Test", new SpriteAnimation(L"Data/Animation/test.txt", true));
	an->PlayAnim("Test");

	//�r�w�C�r�A
	pObj1->AddComponent<Move>(new Move());
	pObj1->AddComponent<Scale>(new Scale());

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
