#include "../../../framework.h"
#include "../../../environment.h"

GameObjectManager::~GameObjectManager() {
	for (auto& obj : umObjects) {
		delete obj.second;
	}
	umObjects.clear();
}

void GameObjectManager::Execute() {
	//BehaviourのStart関数実行
	if (isStartFucnEnable) {
		for (auto& bh : umBehaviour) {
			if (!CheckComponentEnable(bh.second)) continue;
			bh.second->Execute();
		}
		isStartFucnEnable = false;
	}

	//Update
	for (auto& bh : umBehaviour) {
		if (!CheckComponentEnable(bh.second)) continue;
		bh.second->Execute((int)eBehaviourState::Update);
	}

	//Animator
	for (auto& an : umAnimator) {
		if (!CheckComponentEnable(an.second)) continue;
		an.second->Execute();
	}

	//Physics&Collider
	//PhysicsがあるColliderをコンポーネント一覧から探し他のコライダーとの衝突を判定
	for (auto& ph : umPhysics2D) {
		if (!CheckComponentEnable(ph.second)) continue;
		for (auto& com : ph.second->gameObject->components) {
			if (com->type == eComponentType::Collider) {
				noDel_ptr<Component> col = noDel_ptr<Component>(com);
				if (col->IsEnable() == false) continue;
				col->Execute();
				for (auto& c2d : umCollider2D) {
					if (!CheckComponentEnable(c2d.second)) continue;
					if (col == c2d.second) continue;
					c2d.second->Execute();
					col->Execute(static_noDel_cast<Collider2D>(c2d.second));
				}
			}
		}
	}

	//衝突時の処理
	for (auto& bh : umBehaviour) {
		if (!CheckComponentEnable(bh.second)) continue;
		bh.second->Execute();
	}

	//Destroy
	if (vDestroyID.size() != 0) {
		for (int id : vDestroyID) {
			PullOutComponent(noDel_ptr<GameObject>(umObjects[id]));
			delete umObjects[id];
			umObjects.erase(id);
		}
		vDestroyID.clear();
	}

	//オブジェクトのTransformを更新
	for (auto& tr : umTransform) {
		if (!CheckComponentEnable(tr.second)) continue;
		tr.second->Execute((int)eTransformState::ConvertLocalToGlobal);
	}
	for (auto& tr : umTransform) {
		if (!CheckComponentEnable(tr.second)) continue;
		tr.second->Execute((int)eTransformState::UpdateRelativeState);
	}

	//スプライトの描画順の変更
	if (isSortEnable) {
		RenderOrderSort(0, (int)v2DRenderer.size() - 1);
		isSortEnable = false;
	}
}

void GameObjectManager::Render() {
	//メッシュの描画
	for (auto& mesh : umMeshRenderer) {
		if (!CheckComponentEnable(mesh.second)) continue;
		mesh.second->Execute();
	}

	///スプライトの描画
	for (auto& sprite : v2DRenderer) {
		if (!CheckComponentEnable(sprite)) continue;
		sprite->Execute();
	}
}

//オブジェクトの作成
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z, noDel_ptr<Transform> parent, bool local) {
	GameObject* instance = new GameObject();
	instance->SetSceneType(sceneType);
	//Transformの作成
	instance->AddComponent<Transform>();
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	instance->transform->SetUpTransform(x, y, z, parent);
	umObjects[instance->GetInstanceID()] = instance;
	return noDel_ptr<GameObject>(instance);
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float width, float height,
	noDel_ptr<Sprite> sprite, noDel_ptr<Transform> parent, bool local)
{
	GameObject* instance = new GameObject();
	instance->SetSceneType(sceneType);
	//Transformの作成
	instance->AddComponent<Transform>();
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	instance->transform->SetUpTransform(x, y, 0, parent);
	//SpriteRendererの作成
	instance->AddComponent<SpriteRenderer>();
	instance->GetComponent<SpriteRenderer>()->SetUpSpriteRenderer(width, height, sprite);
	umObjects[instance->GetInstanceID()] = instance;
	return noDel_ptr<GameObject>(instance);
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z,
	noDel_ptr<Mesh> mesh, noDel_ptr<Transform> parent, bool local)
{
	GameObject* instance = new GameObject();
	instance->SetSceneType(sceneType);
	//Transformの作成
	instance->AddComponent<Transform>();
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	instance->transform->SetUpTransform(x, y, z, parent);
	//SpriteRendererの作成
	instance->AddComponent<MeshRenderer>();
	instance->GetComponent<MeshRenderer>()->SetUpMeshRenderer(mesh);
	umObjects[instance->GetInstanceID()] = instance;
	return noDel_ptr<GameObject>(instance);
}

//オブジェクトの破棄準備
void GameObjectManager::ReserveDestroyObject(int objID) {
	vDestroyID.emplace_back(objID);
}

//配列から特定の要素を抜き出す
void GameObjectManager::PullOutComponent(noDel_ptr<GameObject> obj) {
	std::unordered_map<int, noDel_ptr<Component>>* umap = nullptr;
	bool isOnce = true;
	for (Component* com : obj->components) {
		if (com->type == eComponentType::Transform) umap = &umTransform;
		else if (com->type == eComponentType::SpriteRenderer || com->type == eComponentType::Font) {
			for (int i = 0; i < v2DRenderer.size(); i++) {
				if (v2DRenderer[i]->GetInstanceID() == com->GetInstanceID()) {
					v2DRenderer.erase(v2DRenderer.begin() + i);
					isSortEnable = true;
					break;
				}
			}
			continue;
		}
		else if (com->type == eComponentType::MeshRenderer) umap = &umMeshRenderer;
		else if (com->type == eComponentType::Collider) umap = &umCollider2D;
		else if (com->type == eComponentType::Physics) umap = &umPhysics2D;
		else if (com->type == eComponentType::Behaviour) {
			umap = &umBehaviour;
			isOnce = false;
		}
		else if (com->type == eComponentType::Animator) umap = &umAnimator;
		else continue;

		auto itr = umap->begin();
		while (itr != umap->end()) {
			if (com->GetInstanceID() == itr->first) {
				auto nextItr = std::next(itr, 1);
				umap->erase(itr->first);
				itr = nextItr;
				if (isOnce) break;
			}
			else {
				++itr;
			}
		}
	}
}

//コンポーネントの登録と各種初期設定(コンストラクタ時には読み取れないgameObjectが使えるためここで設定)
void GameObjectManager::RegistComponent(noDel_ptr<Component> com) {
	std::unordered_map<int, noDel_ptr<Component>>* umap = nullptr;
	if (com->IsRegisted()) return;
	com->SetRegistState(true); //登録状況を変更
	if (com->type == eComponentType::Transform) umap = &umTransform;
	else if (com->type == eComponentType::SpriteRenderer || com->type == eComponentType::Font) {
		v2DRenderer.emplace_back(com);
		isSortEnable = true;
		return;
	}
	else if (com->type == eComponentType::MeshRenderer) umap = &umMeshRenderer;
	else if (com->type == eComponentType::Collider) umap = &umCollider2D;
	else if (com->type == eComponentType::Physics) umap = &umPhysics2D;
	else if (com->type == eComponentType::Behaviour) {
		umap = &umBehaviour;
		isStartFucnEnable = true;
		MessageSystem::SendMessageToCom(noDel_ptr<Component>(com), L"Start");
	}
	else if (com->type == eComponentType::Animator) umap = &umAnimator;
	else return;

	(*umap)[com->GetInstanceID()] = com;
}

//コンポーネント有効無効判定
bool GameObjectManager::CheckComponentEnable(noDel_ptr<Component> com) {
	if (com->gameObject->IsObjEnable() == false) return false;
	if (com->transform->IsParentObjEnable() == false) return false;
	if (com->IsEnable() == false) return false;
	return true;
}

//描画順変更
void GameObjectManager::RenderOrderSort(int start, int end) {
	int left = start;
	int right = end;
	if (left >= right) return;

	//走査を気にしてStaticCastにしている
	float pivot = v2DRenderer[left]->transform->position.z;

	while (true) {
		while (v2DRenderer[left]->transform->position.z < pivot) left++;
		while (v2DRenderer[right]->transform->position.z > pivot) right--;

		if (left < right) {
			noDel_ptr<Component> temp = v2DRenderer[left];
			v2DRenderer[left] = v2DRenderer[right];
			v2DRenderer[right] = temp;

			left++;
			right--;
		}
		else {
			break;
		}
	}

	// 左側再帰
	RenderOrderSort(start, left - 1);

	RenderOrderSort(right + 1, end);
}