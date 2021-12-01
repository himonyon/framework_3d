#include "../../../framework.h"
#include "../../../environment.h"

Component::Component(eComponentType type) {
	this->type = type;
}

void Component::SetEnable(bool flag) {
	if (flag) {
		if (type == eComponentType::Behaviour) {
			SceneManager::GetScene(gameObject->GetSceneType())->SetStartFuncEnable();
			MessageSystem::SendMessageToCom(noDel_ptr<Component>(this), L"Start");
		}
	}
	isEnable = flag;
}
void Component::SetRegistState(bool flag) {
	isRegisted = flag;
}