#include "../../../framework.h"
#include "../../../environment.h"

GameObjectManager::~GameObjectManager() {
	for (auto& obj : objects) {
		delete obj;
	}
	objects.clear();
}

void GameObjectManager::Execute() {
	//コンポーネントを追加
	if (isAddComponent){
		for (auto& obj : objects) {
			if (obj->isAddNewComponent) {
				RegistComponent(noDel_ptr<GameObject>(obj));
			}
		}
	}

	//BehaviourのStart関数実行
	if (isStartFucnEnable) {
		for (auto& bh : vBehaviour) {
			if (!CheckComponentEnable(bh)) continue;
			bh->Execute();
		}
		isStartFucnEnable = false;
	}

	//Update
	for (auto& bh : vBehaviour) {
		if (!CheckComponentEnable(bh)) continue;
		MessageSystem::SendMessageToCom(bh, L"Update");
		bh->Execute();
	}

	//Animator
	for (auto& an : vAnimator) {
		if (!CheckComponentEnable(an)) continue;
		an->Execute();
	}

	//Physics&Collider
	//PhysicsがあるColliderをコンポーネント一覧から探し他のコライダーとの衝突を判定
	for (auto& ph : vPhysics2D) {
		if (!CheckComponentEnable(ph)) continue;
		for (auto& com : ph->gameObject->components) {
			if (com->type == eComponentType::Collider) {
				noDel_ptr<Component> col = noDel_ptr<Component>(com);
				if (col->IsEnable() == false) continue;
				for (auto& c2d : vCollider2D) {
					if (!CheckComponentEnable(c2d)) continue;
					if (col == c2d) continue;
					col->Execute(static_noDel_cast<Collider2D>(c2d));
				}
			}
		}
		
	}

	//衝突時の処理
	for (auto& bh : vBehaviour) {
		if (!CheckComponentEnable(bh)) continue;
		bh->Execute();
	}

	//Destroy
	if (vDestroyID.size() != 0) {
		int _count = 0;
		for (int i = 0; i < objects.size(); i++) {
			if (vDestroyID.size() == _count) break;
			bool flag = false;
			for (int id : vDestroyID) {
				if (objects[i]->GetObjectID() == id) flag = true;
			}

			//削除対象オブジェクトがあればコンポーネントとオブジェクトを配列から抜きオブジェクトDelete
			if (flag) {
				_count++;
				PullOutComponent(noDel_ptr<GameObject>(objects[i]));
				GameObject* deleteObj = objects[i];
				objects.erase(objects.begin() + i);
				delete deleteObj;
				--i;
			}
		}
		vDestroyID.clear();
	}

	//オブジェクトのTransformを更新
	for (auto& tr : vTransform) {
		if (!CheckComponentEnable(tr)) continue;
		MessageSystem::SendMessageToCom(tr, L"ConvertLG");
		tr->Execute();
	}
	for (auto& tr : vTransform) {
		if (!CheckComponentEnable(tr)) continue;
		MessageSystem::SendMessageToCom(tr, L"UpdateTrans");
		tr->Execute();
	}

	//スプライトの描画順の変更
	if (isSpriteSortEnable) {
		RenderOrderSort(0, (int)vSpriteRenderer.size() - 1);
		isSpriteSortEnable = false;
	}
}

void GameObjectManager::Render() {
	//メッシュの描画
	for (auto& mesh : vMeshRenderer) {
		if (!CheckComponentEnable(mesh)) continue;
		mesh->Execute();
	}

	///スプライトの描画
	for (auto& sprite : vSpriteRenderer) {
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
	instance->transform->SetUpTransform(x,y,z,parent);
	objects.emplace_back(instance);
	return noDel_ptr<GameObject>(objects.back());
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
	objects.emplace_back(instance);
	return noDel_ptr<GameObject>(objects.back());
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
	objects.emplace_back(instance);
	return noDel_ptr<GameObject>(objects.back());
}

//オブジェクトの破棄準備
void GameObjectManager::ReserveDestroyObject(int objID) {
	vDestroyID.emplace_back(objID);
}

//配列から特定の要素を抜き出す
void GameObjectManager::PullOutComponent(noDel_ptr<GameObject> obj) {
	std::vector<noDel_ptr<Component>>* vec = nullptr;
	bool isOnce = true;
	for (Component* com : obj->components) {
		if (com->type == eComponentType::Transform) vec = &vTransform;
		else if (com->type == eComponentType::SpriteRenderer) vec = &vSpriteRenderer;
		else if (com->type == eComponentType::MeshRenderer) vec = &vMeshRenderer;
		else if (com->type == eComponentType::Collider) vec = &vCollider2D;
		else if (com->type == eComponentType::Physics) vec = &vPhysics2D;
		else if (com->type == eComponentType::Behaviour) {
			vec = &vBehaviour;
			isOnce = false;
		}
		else if (com->type == eComponentType::Animator) vec = &vAnimator;
		else continue;
		
		for (int i = 0; i < vec->size(); i++) {
			if (com->GetObjectID() == (*vec)[i]->GetObjectID()) {
				vec->erase(vec->begin() + i);
				if (isOnce) break;
				else --i;
			}
		}
	}
}

//コンポーネントの登録と各種初期設定(コンストラクタ時には読み取れないgameObjectが使えるためここで設定)
void GameObjectManager::RegistComponent(noDel_ptr<GameObject> obj) {
	std::vector<noDel_ptr<Component>>* vec = nullptr;
	for (Component* com : obj->components) {
		if (com->IsRegisted()) continue;
		com->SetRegistState(true); //登録状況を変更
		if (com->type == eComponentType::Transform) vec = &vTransform;
		else if (com->type == eComponentType::SpriteRenderer) vec = &vSpriteRenderer;
		else if (com->type == eComponentType::MeshRenderer) vec = &vMeshRenderer;
		else if (com->type == eComponentType::Collider) vec = &vCollider2D;
		else if (com->type == eComponentType::Physics) vec = &vPhysics2D;
		else if (com->type == eComponentType::Behaviour) {
			vec = &vBehaviour;
			isStartFucnEnable = true;
			MessageSystem::SendMessageToCom(noDel_ptr<Component>(com), L"Start");
		}
		else if (com->type == eComponentType::Animator) vec = &vAnimator;
		else continue;

		vec->emplace_back(com);
	}
	obj->isAddNewComponent = false;
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

	while (vSpriteRenderer[left] == NULL) {
		left++;
	}
	if (right <= left) return;

	//走査を気にしてStaticCastにしている
	int pivot = static_noDel_cast<SpriteRenderer>(vSpriteRenderer[left])->GetRenderPriority();

	while (true) {
		while (vSpriteRenderer[left] == NULL) left++;
		while (vSpriteRenderer[right] == NULL) right--;
		while (static_noDel_cast<SpriteRenderer>(vSpriteRenderer[left])->GetRenderPriority() < pivot) left++;
		while (static_noDel_cast<SpriteRenderer>(vSpriteRenderer[right])->GetRenderPriority() > pivot) right--;

		if (left < right) {
			noDel_ptr<Component> temp = vSpriteRenderer[left];
			vSpriteRenderer[left] = vSpriteRenderer[right];
			vSpriteRenderer[right] = temp;

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