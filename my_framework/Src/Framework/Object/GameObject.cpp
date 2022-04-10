#include "../../../framework.h"
#include "../../../environment.h"

GameObject::GameObject(std::string name) {
	objEnable = true;
	if (name == "") this->name = std::to_string(GetInstanceID());
	else this->name = name;
}

GameObject::~GameObject() {
	for (auto& com : components) {
		delete com;
	}
}

void GameObject::Destroy(noDel_ptr<GameObject> obj) {
	if (obj != nullptr) SceneManager::GetScene(belongSceneType)->ReserveDestroyObject(obj->GetName());
	else SceneManager::GetScene(belongSceneType)->ReserveDestroyObject(GetName());
}

//空オブジェクト作成
noDel_ptr<GameObject> GameObject::CreateObject(float x, float y, float z,
	noDel_ptr<Transform> parent, std::string name) {
	return SceneManager::GetScene(belongSceneType)->CreateObject(x, y, z, parent, name);
}
//スプライトオブジェクト作成
noDel_ptr<GameObject> GameObject::CreateObject(float x, float y, float z, noDel_ptr<Sprite> sprite,
	noDel_ptr<Transform> parent, std::string name) {
	return SceneManager::GetScene(belongSceneType)->CreateObject(x, y, z, sprite, parent, name);
}
//メッシュオブジェクト作成
noDel_ptr<GameObject> GameObject::CreateObject(float x, float y, float z, noDel_ptr<Mesh> mesh,
	noDel_ptr<Transform> parent, std::string name) {
	return SceneManager::GetScene(belongSceneType)->CreateObject(x, y, z, mesh, parent, name);
}
//イメージオブジェクト作成
noDel_ptr<GameObject> GameObject::CreateImageObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite,
	noDel_ptr<Transform> parent, std::string name) {
	return SceneManager::GetScene(belongSceneType)->CreateImageObject(x, y, width, height, sprite, parent, name);
}

//オブジェクトの検索
noDel_ptr<GameObject> GameObject::FindGameObject(std::string name) {
	return SceneManager::GetScene(belongSceneType)->GetGameObject(name);
}

void GameObject::SetObjEnable(bool flag) {
	//Behaviour::Update関数での即時実行を防ぐため情報を管理クラスに渡す
	if(flag) SceneManager::GetScene(belongSceneType)->ReserveEnableObject(name);

	objEnable = flag;
	if (flag) {
		for (auto& com : components) {
			if (com->type == eComponentType::Behaviour) {
				SceneManager::GetScene(belongSceneType)->SetStartFuncEnable();
				MessageSystem::SendMessageToCom(noDel_ptr<Component>(com), L"Start");
			}
		}
	}
}

bool GameObject::IsObjEnable() {
	return objEnable;
}

