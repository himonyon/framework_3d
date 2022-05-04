#include "../../../framework.h"
#include "../../../environment.h"

using namespace MyFrameWork;

GameObjectManager::~GameObjectManager() {
	for (auto& obj : umObjects) {
		delete obj.second;
	}
	umObjects.clear();
}

void GameObjectManager::Execute() {
	//Update & Start
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
	//衝突の解消
	for (auto& c2d : umCollider2D) {
		if (!CheckComponentEnable(c2d.second)) continue;
		c2d.second->Execute();
	}
	//PhysicsがあるColliderをコンポーネント一覧から探し他のコライダーとの衝突を判定
	for (auto& ph : umPhysics2D) {
		if (!CheckComponentEnable(ph.second)) continue;
		for (auto& com : ph.second->gameObject->components) {
			if (com->type == eComponentType::Collider) {
				noDel_ptr<Component> col = noDel_ptr<Component>(com);
				if (col->IsEnable() == false) continue;
				for (auto& c2d : umCollider2D) {
					if (!CheckComponentEnable(c2d.second)) continue;
					if (col == c2d.second) continue;
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

	//オブジェクトの有効コンテナの消去
	vEnableName.clear();

	//Destroy
	if (vDestroyName.size() != 0) {
		for (std::string name : vDestroyName) {
			PullOutComponent(noDel_ptr<GameObject>(umObjects[name]));
			delete umObjects[name];
			umObjects.erase(name);
		}
		vDestroyName.clear();
	}

	//オブジェクトのTransformを更新
	for (auto& tr : umTransform) {
		tr.second->Execute((int)eTransformState::ConvertLocalToGlobal);
	}
	for (auto& tr : umTransform) {
		tr.second->Execute((int)eTransformState::UpdateRelativeState);
	}

	//スプライトの描画順の変更
	if (isSortEnable) {
		RenderOrderSort(vImageRenderer ,0, (int)vImageRenderer.size() - 1);
		RenderOrderSort(vSpriteRenderer ,0, (int)vSpriteRenderer.size() - 1);
		RenderOrderSort(vMeshRenderer ,0, (int)vMeshRenderer.size() - 1);
		isSortEnable = false;
	}
}

void GameObjectManager::Render() {
	///背面UIオブジェクトの描画
	for (auto& image : vImageRenderer) {
		if (image->gameObject->IsFrontObj()) continue;
		if (!CheckComponentEnable(image)) continue;
		image->Execute();
	}

	//スプライトの描画
	for (auto& sprite : vSpriteRenderer) {
		if (!CheckComponentEnable(sprite)) continue;
		sprite->Execute();
	}

	//メッシュの描画
	for (auto& mesh : vMeshRenderer) {
		if (!CheckComponentEnable(mesh)) continue;
		mesh->Execute();
	}

	///前面UIオブジェクトの描画
	for (auto& image : vImageRenderer) {
		if (image->gameObject->IsFrontObj() == false) continue;
		if (!CheckComponentEnable(image)) continue;
		image->Execute();
	}
}

//オブジェクトの作成
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z, noDel_ptr<Transform> parent, std::string name) {
	GameObject* _instance = new GameObject(name);
	_instance->SetSceneType(sceneType);
	//Transformの作成
	_instance->AddComponent<Transform>();
	_instance->transform = noDel_ptr<Transform>(_instance->GetComponent<Transform>());
	_instance->transform->SetUpTransform(x, y, z, parent);
	//オブジェクト登録
	umObjects[_instance->GetName()] = _instance;
	return noDel_ptr<GameObject>(_instance);
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z, noDel_ptr<Sprite> sprite, noDel_ptr<Transform> parent, std::string name)
{
	GameObject* _instance = new GameObject(name);
	_instance->SetSceneType(sceneType);
	//Transformの作成
	_instance->AddComponent<Transform>();
	_instance->transform = noDel_ptr<Transform>(_instance->GetComponent<Transform>());
	_instance->transform->SetUpTransform(x, y, z, parent);
	//SpriteRendererの作成
	_instance->AddComponent<SpriteRenderer>();
	_instance->GetComponent<SpriteRenderer>()->SetUpSpriteRenderer(sprite);
	//オブジェクト登録
	umObjects[_instance->GetName()] = _instance;
	return noDel_ptr<GameObject>(_instance);
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z,
	noDel_ptr<Model> model, noDel_ptr<Transform> parent, std::string name)
{
	int _createCount = 0; //生成カウント

	//空のオブジェクトを作成して親にする
	noDel_ptr<GameObject> _rootObj = CreateObject(x, y, z, parent, name);

	//名前がない場合、空オブジェクトの名前を設定
	if (name == "") name = _rootObj->GetName();

	noDel_ptr<GameObject> _parentObj = _rootObj; //親オブジェクト

	//ルートメッシュとその子メッシュ全てオブジェクト化する
	int _rootNum = model->GetRootCount();
	//ルートメッシュ取得
	for (int i = 0; i < _rootNum; i++) {
		noDel_ptr<Mesh> _mesh = model->GetRootMesh(i);
		if (_mesh == NULL) continue; //データがなければ処理しない
		//ルートメッシュのオブジェクト生成
		std::string _newName = name + "_" + std::to_string(_createCount); //新しい名前
		//座標
		stVector3 _newPos = _mesh->GetInitPos();
		stVector3 _newRot = _mesh->GetInitRot();
		stVector3 _newScl = _mesh->GetInitScl();
		//生成
		noDel_ptr<GameObject> _newObj = CreateObject(_newPos, _newRot, _newScl, _mesh, _parentObj->transform, _newName);
		_parentObj = _newObj;
		_createCount++;
		//子メッシュの生成
		CreateObjectForChildByRootMesh(_mesh, _parentObj->transform, name, _createCount);

		_parentObj = _rootObj; //親オブジェクトを再設定
	}

	return _rootObj;
}
void GameObjectManager::CreateObjectForChildByRootMesh(noDel_ptr<Mesh> mesh,
	noDel_ptr<Transform> parent, std::string name, int& createCount) {
	//子メッシュをオブジェクト化
	int _childCount = mesh->GetChildCount();
	for (int i = 0; i < _childCount; i++) {
		noDel_ptr<Mesh> _child = mesh->GetChild(i);
		if (_child == NULL) continue; //データがなければ処理しない
		//メッシュオブジェクト生成
		std::string _newName = name + "_" + std::to_string(createCount); //新しい名前
		stVector3 _newPos = _child->GetInitPos();
		stVector3 _newRot = _child->GetInitRot();
		stVector3 _newScl = _child->GetInitScl();
		noDel_ptr<GameObject> _newObj = CreateObject(_newPos, _newRot, _newScl, _child, parent, _newName);
		noDel_ptr<GameObject> _parentObj = _newObj; //親オブジェクト
		createCount++; //生成カウントUP
		CreateObjectForChildByRootMesh(_child, _parentObj->transform, name, createCount);
	}
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(stVector3& pos, stVector3& rot, stVector3& scl,
	noDel_ptr<Mesh> mesh, noDel_ptr<Transform> parent, std::string name)
{
	GameObject* _instance = new GameObject(name);
	_instance->SetSceneType(sceneType);
	//Transformの作成
	_instance->AddComponent<Transform>();
	_instance->transform = noDel_ptr<Transform>(_instance->GetComponent<Transform>());
	_instance->transform->SetUpTransform(pos, rot, scl, parent);
	//MeshRendererの作成
	_instance->AddComponent<MeshRenderer>();
	_instance->GetComponent<MeshRenderer>()->SetUpMeshRenderer(mesh);
	//オブジェクト登録
	umObjects[_instance->GetName()] = _instance;
	return noDel_ptr<GameObject>(_instance);
}

noDel_ptr<GameObject> GameObjectManager::CreateImageObject(float x, float y, float width, float height,
	noDel_ptr<Sprite> sprite, noDel_ptr<Transform> parent, std::string name)
{
	GameObject* _instance = new GameObject(name);
	_instance->SetSceneType(sceneType);
	//Transformの作成
	_instance->AddComponent<Transform>();
	_instance->transform = noDel_ptr<Transform>(_instance->GetComponent<Transform>());
	_instance->transform->SetUpTransform(x, y, 0, parent);
	//SpriteRendererの作成
	_instance->AddComponent<ImageRenderer>();
	_instance->GetComponent<ImageRenderer>()->SetUpImageRenderer(width, height, sprite);
	//オブジェクト登録
	umObjects[_instance->GetName()] = _instance;
	return noDel_ptr<GameObject>(_instance);
}

//オブジェクトの破棄準備
void GameObjectManager::ReserveDestroyObject(std::string name) {
	vDestroyName.emplace_back(name);
}
//オブジェクトの有効準備
void GameObjectManager::ReserveEnableObject(std::string name) {
	vEnableName.emplace_back(name);
}

noDel_ptr<GameObject> GameObjectManager::GetGameObject(std::string name) {
	return noDel_ptr<GameObject>(umObjects[name]);
}

//配列から特定の要素を抜き出す
void GameObjectManager::PullOutComponent(noDel_ptr<GameObject> obj) {
	std::unordered_map<int, noDel_ptr<Component>>* umap = nullptr;
	bool isOnce = true; //複数同一コンポーネントがない場合はTRUE
	for (Component* com : obj->components) {
		if (com->type == eComponentType::Transform) umap = &umTransform;
		else if (com->type == eComponentType::UIRenderer) {
			PullOutComponentToVector(vImageRenderer, noDel_ptr<Component>(com));
			continue;
		}
		else if (com->type == eComponentType::SpriteRenderer) {
			PullOutComponentToVector(vSpriteRenderer, noDel_ptr<Component>(com));
			continue;
		}
		else if (com->type == eComponentType::MeshRenderer) {
			PullOutComponentToVector(vMeshRenderer, noDel_ptr<Component>(com));
			continue;
		}
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
				umap->erase(itr);
				itr = nextItr;
				if (isOnce) break;
			}
			else {
				++itr;
			}
		}
	}
}
void GameObjectManager::PullOutComponentToVector(std::vector<noDel_ptr<Component>>& renderer, noDel_ptr<Component> com) {
	for (int i = 0; i < renderer.size(); i++) {
		if (renderer[i]->GetInstanceID() == com->GetInstanceID()) {
			renderer.erase(renderer.begin() + i);
			isSortEnable = true;
			break;
		}
	}
}

//コンポーネントの登録と各種初期設定(コンストラクタ時には読み取れないgameObjectが使えるためここで設定)
void GameObjectManager::RegistComponent(noDel_ptr<Component> com) {
	std::unordered_map<int, noDel_ptr<Component>>* umap = nullptr;

	if (com->IsRegisted()) return; //登録済みなら無視

	com->SetRegistState(true); //登録状況を変更
	if (com->type == eComponentType::Transform) umap = &umTransform;
	else if (com->type == eComponentType::UIRenderer) {
		RegistComponentToVector(vImageRenderer, com);
		return;
	}
	else if (com->type == eComponentType::SpriteRenderer) {
		RegistComponentToVector(vSpriteRenderer, com);
		return;
	}
	else if (com->type == eComponentType::MeshRenderer) {
		RegistComponentToVector(vMeshRenderer, com);
		return;
	}
	else if (com->type == eComponentType::Collider) umap = &umCollider2D;
	else if (com->type == eComponentType::Physics) umap = &umPhysics2D;
	else if (com->type == eComponentType::Behaviour) umap = &umBehaviour;
	else if (com->type == eComponentType::Animator) umap = &umAnimator;
	else return;

	(*umap)[com->GetInstanceID()] = com;

	//Behaviourの場合Awakeを呼び出す
	if (com->type == eComponentType::Behaviour) {
		com->Execute((int)eBehaviourState::Awake);
	}
}

void GameObjectManager::RegistComponentToVector(std::vector<noDel_ptr<Component>>& renderer, noDel_ptr<Component> com) {
	renderer.emplace_back(com);
	isSortEnable = true;
}

//コンポーネント有効無効判定
bool GameObjectManager::CheckComponentEnable(noDel_ptr<Component> com) {
	//このフレームで有効になっている場合無視
	bool _flag = false;
	for (auto& name : vEnableName) {
		if (com->gameObject->GetName() == name) return false;
	}
	if (com->gameObject->IsObjEnable() == false) return false;
	if (com->transform->IsParentObjEnable() == false) return false;
	if (com->IsEnable() == false) return false;
	return true;
}

//描画順変更
void GameObjectManager::RenderOrderSort(std::vector<noDel_ptr<Component>>& renderer, int start, int end) {
	int left = start;
	int right = end;
	if (left >= right) return;

	//走査を気にしてStaticCastにしている
	int pivot = renderer[left]->gameObject->GetRenderOrder();

	while (true) {
		while (renderer[left]->gameObject->GetRenderOrder() < pivot) left++;
		while (renderer[right]->gameObject->GetRenderOrder() > pivot) right--;

		if (left < right) {
			noDel_ptr<Component> temp = renderer[left];
			renderer[left] = renderer[right];
			renderer[right] = temp;

			left++;
			right--;
		}
		else {
			break;
		}
	}

	// 左側再帰
	RenderOrderSort(renderer, start, left - 1);

	RenderOrderSort(renderer, right + 1, end);
}