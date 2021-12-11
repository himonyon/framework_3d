#include "../../../framework.h"
#include "../../../environment.h"

GameObjectManager::~GameObjectManager() {
	for (auto& obj : umObjects) {
		delete obj.second;
	}
	umObjects.clear();
}

void GameObjectManager::Execute() {
	//�R���|�[�l���g��ǉ�
	if (isAddComponent){
		for (auto& obj : umObjects) {
			if (obj.second->isAddNewComponent) {
				RegistComponent(noDel_ptr<GameObject>(obj.second));
			}
		}
	}

	//Behaviour��Start�֐����s
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
	//Physics������Collider���R���|�[�l���g�ꗗ����T�����̃R���C�_�[�Ƃ̏Փ˂𔻒�
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

	//�Փˎ��̏���
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

	//�I�u�W�F�N�g��Transform���X�V
	for (auto& tr : umTransform) {
		if (!CheckComponentEnable(tr.second)) continue;
		tr.second->Execute((int)eTransformState::ConvertLocalToGlobal);
	}
	for (auto& tr : umTransform) {
		if (!CheckComponentEnable(tr.second)) continue;
		tr.second->Execute((int)eTransformState::UpdateRelativeState);
	}

	//�X�v���C�g�̕`�揇�̕ύX
	if (isSpriteSortEnable) {
		RenderOrderSort(0, (int)vSpriteRenderer.size() - 1);
		isSpriteSortEnable = false;
	}
}

void GameObjectManager::Render() {
	//���b�V���̕`��
	for (auto& mesh : umMeshRenderer) {
		if (!CheckComponentEnable(mesh.second)) continue;
		mesh.second->Execute();
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
	umObjects[instance->GetInstanceID()] = instance;
	return noDel_ptr<GameObject>(instance);
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
	umObjects[instance->GetInstanceID()] = instance;
	return noDel_ptr<GameObject>(instance);
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
	umObjects[instance->GetInstanceID()] = instance;
	return noDel_ptr<GameObject>(instance);
}

//�I�u�W�F�N�g�̔j������
void GameObjectManager::ReserveDestroyObject(int objID) {
	vDestroyID.emplace_back(objID);
}

//�z�񂩂����̗v�f�𔲂��o��
void GameObjectManager::PullOutComponent(noDel_ptr<GameObject> obj) {
	std::unordered_map<int, noDel_ptr<Component>>* umap = nullptr;
	bool isOnce = true;
	for (Component* com : obj->components) {
		if (com->type == eComponentType::Transform) umap = &umTransform;
		else if (com->type == eComponentType::SpriteRenderer) {
			for (int i = 0; i < vSpriteRenderer.size(); i++) {
				if (vSpriteRenderer[i]->GetInstanceID() == com->GetInstanceID()) {
					vSpriteRenderer.erase(vSpriteRenderer.begin() + i);
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

//�R���|�[�l���g�̓o�^�Ɗe�평���ݒ�(�R���X�g���N�^���ɂ͓ǂݎ��Ȃ�gameObject���g���邽�߂����Őݒ�)
void GameObjectManager::RegistComponent(noDel_ptr<GameObject> obj) {
	std::unordered_map<int, noDel_ptr<Component>>* umap = nullptr;
	for (Component* com : obj->components) {
		if (com->IsRegisted()) continue;
		com->SetRegistState(true); //�o�^�󋵂�ύX
		if (com->type == eComponentType::Transform) umap = &umTransform;
		else if (com->type == eComponentType::SpriteRenderer) {
			vSpriteRenderer.emplace_back(com);
			continue;
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
		else continue;

		(*umap)[com->GetInstanceID()] = noDel_ptr<Component>(com);
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
	if (left >= right) return;

	//�������C�ɂ���StaticCast�ɂ��Ă���
	int pivot = static_noDel_cast<SpriteRenderer>(vSpriteRenderer[left])->GetRenderPriority();

	while (true) {
		while (static_noDel_cast<SpriteRenderer>(vSpriteRenderer[left])->GetRenderPriority() < pivot) left++;
		while (static_noDel_cast<SpriteRenderer>(vSpriteRenderer[right])->GetRenderPriority() > pivot) right--;

		if (left < right) {
			noDel_ptr<SpriteRenderer> temp = static_noDel_cast<SpriteRenderer>(vSpriteRenderer[left]);
			vSpriteRenderer[left] = vSpriteRenderer[right];
			vSpriteRenderer[right] = static_noDel_cast<Component>(temp);

			left++;
			right--;
		}
		else {
			break;
		}
	}

	// �����ċA
	RenderOrderSort(start, left - 1);
	
	RenderOrderSort(right + 1, end);
}