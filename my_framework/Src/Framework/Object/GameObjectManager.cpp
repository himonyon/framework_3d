#include "../../../framework.h"
#include "../../../environment.h"

GameObjectManager::~GameObjectManager() {
	for (auto& obj : objects) {
		delete obj;
	}
	objects.clear();
}

void GameObjectManager::Execute() {
	//�R���|�[�l���g��ǉ�
	if (isAddComponent){
		for (auto& obj : objects) {
			if (obj->isAddNewComponent) {
				RegistComponent(noDel_ptr<GameObject>(obj));
			}
		}
	}

	//Behaviour��Start�֐����s
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
	//Physics������Collider���R���|�[�l���g�ꗗ����T�����̃R���C�_�[�Ƃ̏Փ˂𔻒�
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

	//�Փˎ��̏���
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

			//�폜�ΏۃI�u�W�F�N�g������΃R���|�[�l���g�ƃI�u�W�F�N�g��z�񂩂甲���I�u�W�F�N�gDelete
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

	//�I�u�W�F�N�g��Transform���X�V
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

	//�X�v���C�g�̕`�揇�̕ύX
	if (isSpriteSortEnable) {
		RenderOrderSort(0, (int)vSpriteRenderer.size() - 1);
		isSpriteSortEnable = false;
	}
}

void GameObjectManager::Render() {
	//���b�V���̕`��
	for (auto& mesh : vMeshRenderer) {
		if (!CheckComponentEnable(mesh)) continue;
		mesh->Execute();
	}

	///�X�v���C�g�̕`��
	for (auto& sprite : vSpriteRenderer) {
		if (!CheckComponentEnable(sprite)) continue;
		sprite->Execute();
	}
}

//�I�u�W�F�N�g�̍쐬
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z, noDel_ptr<Transform> parent, bool local) {
	GameObject* instance = new GameObject();
	instance->SetSceneType(sceneType);
	//Transform�̍쐬
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
	//Transform�̍쐬
	instance->AddComponent<Transform>();
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	instance->transform->SetUpTransform(x, y, 0, parent);
	//SpriteRenderer�̍쐬
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
	//Transform�̍쐬
	instance->AddComponent<Transform>();
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	instance->transform->SetUpTransform(x, y, z, parent);
	//SpriteRenderer�̍쐬
	instance->AddComponent<MeshRenderer>();
	instance->GetComponent<MeshRenderer>()->SetUpMeshRenderer(mesh);
	objects.emplace_back(instance);
	return noDel_ptr<GameObject>(objects.back());
}

//�I�u�W�F�N�g�̔j������
void GameObjectManager::ReserveDestroyObject(int objID) {
	vDestroyID.emplace_back(objID);
}

//�z�񂩂����̗v�f�𔲂��o��
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

//�R���|�[�l���g�̓o�^�Ɗe�평���ݒ�(�R���X�g���N�^���ɂ͓ǂݎ��Ȃ�gameObject���g���邽�߂����Őݒ�)
void GameObjectManager::RegistComponent(noDel_ptr<GameObject> obj) {
	std::vector<noDel_ptr<Component>>* vec = nullptr;
	for (Component* com : obj->components) {
		if (com->IsRegisted()) continue;
		com->SetRegistState(true); //�o�^�󋵂�ύX
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

//�R���|�[�l���g�L����������
bool GameObjectManager::CheckComponentEnable(noDel_ptr<Component> com) {
	if (com->gameObject->IsObjEnable() == false) return false;
	if (com->transform->IsParentObjEnable() == false) return false;
	if (com->IsEnable() == false) return false;
	return true;
}

//�`�揇�ύX
void GameObjectManager::RenderOrderSort(int start, int end) {
	int left = start;
	int right = end;

	while (vSpriteRenderer[left] == NULL) {
		left++;
	}
	if (right <= left) return;

	//�������C�ɂ���StaticCast�ɂ��Ă���
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

	// �����ċA
	RenderOrderSort(start, left - 1);
	// �E���ċA
	RenderOrderSort(right + 1, end);
}