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

//��I�u�W�F�N�g�쐬
noDel_ptr<GameObject> GameObject::CreateObject(float x, float y, float z,
	noDel_ptr<Transform> parent, std::string name) {
	return SceneManager::GetScene(belongSceneType)->CreateObject(x, y, z, parent, name);
}
//�X�v���C�g�I�u�W�F�N�g�쐬
noDel_ptr<GameObject> GameObject::CreateObject(float x, float y, float z, noDel_ptr<Sprite> sprite,
	noDel_ptr<Transform> parent, std::string name) {
	return SceneManager::GetScene(belongSceneType)->CreateObject(x, y, z, sprite, parent, name);
}
//���b�V���I�u�W�F�N�g�쐬
noDel_ptr<GameObject> GameObject::CreateObject(float x, float y, float z, noDel_ptr<Model> model,
	noDel_ptr<Transform> parent, std::string name) {
	return SceneManager::GetScene(belongSceneType)->CreateObject(x, y, z, model, parent, name);
}
//�C���[�W�I�u�W�F�N�g�쐬
noDel_ptr<GameObject> GameObject::CreateImageObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite,
	noDel_ptr<Transform> parent, std::string name) {
	return SceneManager::GetScene(belongSceneType)->CreateImageObject(x, y, width, height, sprite, parent, name);
}

//�I�u�W�F�N�g�̌���
noDel_ptr<GameObject> GameObject::FindGameObject(std::string name) {
	return SceneManager::GetScene(belongSceneType)->GetGameObject(name);
}

void GameObject::SetObjEnable(bool flag) {
	//Behaviour::Update�֐��ł̑������s��h�����ߏ����Ǘ��N���X�ɓn��
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

