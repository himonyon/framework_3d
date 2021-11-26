#include "../../framework.h"
#include "../../environment.h"

//������
bool SceneTitle::Initialize() {
	//�QD�I�u�W�F�N�g
	pSp0 = CreateSprite(L"Data/Image/bg.jpg");

	pObj1 = noDel_ptr<GameObject2D>(CreateObject2D<GameObject2D>(
		new GameObject2D(500, 300, 200, 200, 
			CreateSprite(L"Data/Image/sample.png"))));
	pObj1->SetRenderPriority(2);
	pObj1->pRenderSprite->vtx[0].a = 0.2f;

	pObj2 = noDel_ptr<GameObject2D>(CreateObject2D<GameObject2D>(
		new GameObject2D(100, 300, 200, 200, 
			CreateSprite(L"Data/Image/sample.png"))));

	//�RD�I�u�W�F�N�g
	pTest = noDel_ptr<GameObject3D>(CreateObject3D<GameObject3D>(
		new GameObject3D(0, 0, 5, 
			CreateMesh(L"Data/Object/test.obj"))));

	pChips = noDel_ptr<GameObject3D>(CreateObject3D<GameObject3D>(
		new GameObject3D(0, 0, 5,
			CreateMesh(L"Data/Object/Chips.obj"), true, pTest)));

	pTest_mesh = CreateMesh(L"Data/Object/test.obj");

	//�T�E���h
	pSound0 = CreateSound(L"Data/Sound/title_bgm.wav");
	pSound0->Play();

	//�A�j���[�V����
	pAnim = CreateAnimation(pObj2, true);
	pAnim->SetKeyFrameRot(0, 0, 0);
	pAnim->SetKeyFrameRot(1, 3.14f, 30);
	pAnim->SetKeyFrameSprite(1, pSp0);
	pAnim->SetKeyFrameRot(2, 6.24f, 60);
	pAnim->SetKeyFrameRot(3, 6.24f, 60);
	pAnim->SetAnimEnable(true);
	return true;
}

void SceneTitle::Terminate() {
}

//����
void SceneTitle::Execute() {
	if (Input::Trg(InputConfig::decide)) {
		SceneManager::SwitchScene(eSceneTable::Game);
	}

	if (IsCollide2D(pObj1, pObj2) != eCollideState::None) {
		Font::Print(600,600,L"�������Ă��");
	}

	if (Joystick::GetRX()) {
		pObj1->rot.y += Joystick::GetRX();
	}
	else if (Joystick::GetRY()) {
		pObj1->rot.x += Joystick::GetRY();
	}

	if ((Joystick::GetLX() || Joystick::GetLY()) && Joystick::On(JOY_L) && Joystick::On(JOY_SQUARE)) {
		pTest->scale.x += Joystick::GetLX() / 20;
		pTest->scale.y -= Joystick::GetLY() / 20;
	}
	else if((Joystick::GetLX() || Joystick::GetLY()) && Joystick::On(JOY_SQUARE)) {
		pTest->position.x += Joystick::GetLX() / 20;
		pTest->position.y += Joystick::GetLY() / 20;
	}
	else if ((Joystick::GetLX()||Joystick::GetLY()) && Joystick::On(JOY_L)) {
		pObj1->scale.x += Joystick::GetLX();
		pObj1->scale.y -= Joystick::GetLY();
	}
	else {
		pObj1->position.x += Joystick::GetLX();
		pObj1->position.y += Joystick::GetLY();
	}

	Font::Print(900, 500, L"%f, %f", Joystick::GetLX(), Joystick::GetLY());
	Font::SetRect(200, 100, 500, WINDOW_HEIGHT);
	Font::SetTextAlignment(eTextAlignment::Center);
	Font::SetColor(0xffffffff);
	Font::Print(500, 200, L"�ύX��@%d ", Joystick::PovOn(18000));
	Font::SetColor(0xff88ff88);
	Font::Print(200, 130, L"vvv�@%d ", Joystick::PovOn(18000));
	Font::Print(200, 160, L"vvv�@%d ", Joystick::PovOn(18000));
	Font::SetRect();

	Scene::Execute();
}

//�`��
void SceneTitle::Render() {
	Scene::Render();
}
