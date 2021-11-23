#pragma once

class SceneTitle : public Scene {
private:
	noDel_ptr<Sprite> pSp0;
	noDel_ptr<GameObject2D> pObj1;
	noDel_ptr<Sound> pSound0;
	noDel_ptr<GameObject2D> pObj2;
	noDel_ptr<SpriteAnimation> pAnim;

	noDel_ptr<GameObject3D> pChips;
	noDel_ptr<GameObject3D> pTest;
	noDel_ptr<Mesh> pTest_mesh;

private:
	//初期化
	bool Initialize();

	//削除
	void Terminate();

	//処理
	void Execute();

	//描画
	void Render();
};