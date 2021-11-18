#include "../../../framework.h"
#include "../../../environment.h"

GameObjectManager::~GameObjectManager() {
	for (auto& sprite : objects2d) {
		delete sprite;
	}
	objects2d.clear();
}

noDel_ptr<GameObject2D> GameObjectManager::CreateObject2D(GameObject2D* instance) {
	objects2d.emplace_back(instance);
	noDel_ptr<GameObject2D> p = noDel_ptr<GameObject2D>(objects2d.back());
	return p;
}
noDel_ptr<GameObject2D> GameObjectManager::CreateObject2D(float x, float y, float width, float height,
	noDel_ptr<Sprite> sprite, bool isRender, noDel_ptr<GameObject> parent)
{
	GameObject2D* instance = new GameObject2D(x, y, width, height, sprite, isRender, parent);
	objects2d.emplace_back(instance);
	noDel_ptr<GameObject2D> p = noDel_ptr<GameObject2D>(objects2d.back());
	return p;
}

noDel_ptr<GameObject3D> GameObjectManager::CreateObject3D(GameObject3D* instance) {
	objects3d.emplace_back(instance);
	noDel_ptr<GameObject3D> p = noDel_ptr<GameObject3D>(objects3d.back());
	return p;
}
noDel_ptr<GameObject3D> GameObjectManager::CreateObject3D(float x, float y, float z,
	noDel_ptr<Mesh> mesh, bool isRender, noDel_ptr<GameObject> parent)
{
	GameObject3D* instance = new GameObject3D(x, y, z, mesh, isRender, parent);
	objects3d.emplace_back(instance);
	noDel_ptr<GameObject3D> p = noDel_ptr<GameObject3D>(objects3d.back());
	return p;
}

void GameObjectManager::Render() {
	//２Dオブジェクトの描画処理
	for (auto& obj : objects2d) {
		if (obj == nullptr) continue;
		if (obj->isRenderEnable() == false) continue;
		if (obj->GetParent() != nullptr && obj->GetParent()->isRenderEnable() == false) continue;
		obj->Render();
	}

	//３Dオブジェクトの描画処理
	for (auto& obj : objects3d) {
		if (obj == nullptr) continue;
		if (obj->isRenderEnable() == false) continue;
		if (obj->GetParent() != nullptr && obj->GetParent()->isRenderEnable() == false) continue;
		obj->Render();
	}
}

void GameObjectManager::Execute() {
	//２Dオブジェクトの処理
	bool isSorted = false;
	for (auto& obj : objects2d) {
		if (obj == nullptr) continue;
		if (obj->isExecuteEnable() == false) continue;
		if (obj->GetParent() != nullptr && obj->GetParent()->isExecuteEnable() == false) continue;
		obj->Execute();

		//描画順が変更された場合ソート
		if (obj->isSortEnable()) {
			isSorted = true;
			obj->SetSortEnable(false);
		}
	}
	if (isSorted) RenderOrderSort(0, (int)objects2d.size() - 1);

	//３Dオブジェクトの描画処理
	for (auto& obj : objects3d) {
		if (obj == nullptr) continue;
		if (obj->isExecuteEnable() == false) continue;
		if (obj->GetParent() != nullptr && obj->GetParent()->isExecuteEnable() == false) continue;
		obj->Execute();
	}
}

void GameObjectManager::RenderOrderSort(int start, int end) {
	int left = start;
	int right = end;

	while (objects2d[left] == NULL) {
		left++;
	}
	if (right <= left) return;

	int pivot = objects2d[left]->GetRenderPriority();

	while (true) {
		while (objects2d[left] == NULL) left++;
		while (objects2d[right] == NULL) right--;
		while (objects2d[left]->GetRenderPriority() < pivot) left++;
		while (objects2d[right]->GetRenderPriority() > pivot) right--;

		if (left < right) {
			GameObject2D* temp = objects2d[left];
			objects2d[left] = objects2d[right];
			objects2d[right] = temp;

			left++;
			right--;
		}
		else {
			break;
		}
	}

	// 左側再帰
	RenderOrderSort(start, left - 1);
	// 右側再帰
	RenderOrderSort(right + 1, end);
}