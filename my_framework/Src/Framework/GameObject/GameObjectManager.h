#pragma once

class GameObjectManager {
public:
	GameObjectManager() {};
	~GameObjectManager();

	//２Dオブジェクトクリエイター
	noDel_ptr<GameObject2D> CreateObject2D(GameObject2D* instance);
	noDel_ptr<GameObject2D> CreateObject2D(float x, float y, float width, float height,
		noDel_ptr<Sprite> sprite, bool isRender = true, noDel_ptr<GameObject> parent = nullptr);

	//３Dオブジェクトクリエイター
	noDel_ptr<GameObject3D> CreateObject3D(GameObject3D* instance);
	noDel_ptr<GameObject3D> CreateObject3D(float x, float y, float z,
		noDel_ptr<Mesh> mesh, bool isRender = true, noDel_ptr<GameObject> parent = nullptr);

	//処理
	void Execute();
	//描画
	void Render();

private:
	//描画順の変更(クイックソート)
	void RenderOrderSort(int start, int end);

private:
	std::vector<GameObject2D*> objects2d;
	std::vector<GameObject3D*> objects3d;
};
