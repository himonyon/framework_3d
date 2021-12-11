#include "../../../framework.h"
#include "../../../environment.h"

GameObject::GameObject() {
	objEnable = true;
}

 GameObject::~GameObject() {
	for (auto& com : components) {
		delete com;
	}
}

void GameObject::Destroy(noDel_ptr<GameObject> obj) {
	if (obj != nullptr) SceneManager::GetScene(belongSceneType)->ReserveDestroyObject(obj->GetInstanceID());
	else SceneManager::GetScene(belongSceneType)->ReserveDestroyObject(GetInstanceID());
}

//空オブジェクト作成
noDel_ptr<GameObject> GameObject::CreateObject(float x, float y, float z,
	noDel_ptr<Transform> parent, bool local) {
	return SceneManager::GetScene(belongSceneType)->CreateObject(x, y, z, parent, local);
}
//スプライトオブジェクト作成
noDel_ptr<GameObject> GameObject::CreateObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite,
	noDel_ptr<Transform> parent, bool local) {
	return SceneManager::GetScene(belongSceneType)->CreateObject(x, y, width, height, sprite, parent, local);
}
//メッシュオブジェクト作成
noDel_ptr<GameObject> GameObject::CreateObject(float x, float y, float z, noDel_ptr<Mesh> mesh,
	noDel_ptr<Transform> parent, bool local) {
	return SceneManager::GetScene(belongSceneType)->CreateObject(x, y, z, mesh, parent, local);
}

void GameObject::SetObjEnable(bool flag) {
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

