#include "../../../framework.h"
#include "../../../environment.h"

GameObjectManager::~GameObjectManager() {
	for (auto& obj : objects2d) {
		delete obj;
	}
	objects2d.clear();

	for (auto& obj : objects3d) {
		delete obj;
	}
	objects3d.clear();
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
		obj->UpdateObjState();

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
		obj->UpdateObjState();
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