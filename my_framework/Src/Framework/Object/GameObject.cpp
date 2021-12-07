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

