#include "../../framework.h"
#include "../../environment.h"

//初期化
bool SceneTitle::Initialize() {
	//２Dオブジェクト
	pSp0 = CreateSprite(L"Data/Image/bg.jpg");

	pObj1 = noDel_ptr<GameObject2D>(CreateObject2D<GameObject2D>(
		new GameObject2D(500, 300, 200, 200, 
			CreateSprite(L"Data/Image/sample.png"))));
	pObj1->SetRenderPriority(2);
	pObj1->pRenderSprite->vtx[0].a = 0.2f;

	pObj2 = noDel_ptr<GameObject2D>(CreateObject2D<GameObject2D>(
		new GameObject2D(100, 300, 200, 200, 
			CreateSprite(L"Data/Image/sample.png"))));

	//３Dオブジェクト
	pTest = noDel_ptr<GameObject3D>(CreateObject3D<GameObject3D>(
		new GameObject3D(0, 0, 5, 
			CreateMesh(L"Data/Object/test.obj"))));

	pChips = noDel_ptr<GameObject3D>(CreateObject3D<GameObject3D>(
		new GameObject3D(0, 0, 5,
			CreateMesh(L"Data/Object/Chips.obj"), true, pTest)));

	pTest_mesh = CreateMesh(L"Data/Object/test.obj");

	//サウンド
	pSound0 = CreateSound(L"Data/Sound/title_bgm.wav");
	pSound0->Play();

	//アニメーション
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

//処理
void SceneTitle::Execute() {
	if (Input::Trg(InputConfig::decide)) {
		SceneManager::SwitchScene(eSceneTable::Game);
	}

	if (IsCollide2D(pObj1, pObj2) != eCollideState::None) {
		Font::Print(600,600,L"当たってんで");
	}

	if (Joystick::GetRX()) {
		pChips->rot.y += Joystick::GetRX() / 10;
	}
	else if (Joystick::GetRY()) {
		pChips->rot.x += Joystick::GetRY() / 10;
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
		pChips->scale.x += Joystick::GetLX() /20;
		pChips->scale.y -= Joystick::GetLY() /20;
	}
	else {
		pChips->position.x += Joystick::GetLX()/20;
		pChips->position.y += Joystick::GetLY()/20;
	}

	if (Joystick::On(JOY_L2)) {
		pChips->position.z += 0.1f;
	}
	if (Joystick::On(JOY_CROSS)) {
		pChips->position.z -= 0.1f;
	}
	if (Joystick::On(JOY_TRIANGLAR)) {
		pTest->rot.z += 0.1f;
	}
	if (Joystick::Trg(JOY_R2)) {
		pChips->pRenderMesh = pTest_mesh;
	}

	Font::Print(900, 500, L"%f, %f", Joystick::GetLX(), Joystick::GetLY());
	Font::SetRect(200, 100, 500, WINDOW_HEIGHT);
	Font::SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	Font::SetColor(0xffffffff);
	Font::Print(L"変更後　%d ", Joystick::PovOn(18000));
	Font::SetColor(0xff88ff88);
	Font::Print(200, 130, L"vvv　%d ", Joystick::PovOn(18000));
	Font::Print(200, 160, L"vvv　%d ", Joystick::PovOn(18000));
	Font::SetRect();

	Scene::Execute();
}

//描画
void SceneTitle::Render() {
	Scene::Render();
}
