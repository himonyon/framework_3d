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
	//�Փ˂̉���
	for (auto& c2d : umCollider2D) {
		if (!CheckComponentEnable(c2d.second)) continue;
		c2d.second->Execute();
	}
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

	//�I�u�W�F�N�g�̗L���R���e�i�̏���
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

	//�I�u�W�F�N�g��Transform���X�V
	for (auto& tr : umTransform) {
		tr.second->Execute((int)eTransformState::ConvertLocalToGlobal);
	}
	for (auto& tr : umTransform) {
		tr.second->Execute((int)eTransformState::UpdateRelativeState);
	}

	//�X�v���C�g�̕`�揇�̕ύX
	if (isSortEnable) {
		RenderOrderSort(0, (int)v2DRenderer.size() - 1);
		isSortEnable = false;
	}
}

void GameObjectManager::Render() {
	//���b�V���̕`��
	for (auto& mesh : um3DRenderer) {
		if (!CheckComponentEnable(mesh.second)) continue;
		mesh.second->Execute();
	}

	///�X�v���C�g�̕`��
	for (auto& sprite : v2DRenderer) {
		if (!CheckComponentEnable(sprite)) continue;
		sprite->Execute();
	}
}

//�I�u�W�F�N�g�̍쐬
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z, noDel_ptr<Transform> parent, std::string name) {
	GameObject* instance = new GameObject(name);
	instance->SetSceneType(sceneType);
	//Transform�̍쐬
	instance->AddComponent<Transform>();
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	instance->transform->SetUpTransform(x, y, z, parent);
	//�I�u�W�F�N�g�o�^
	umObjects[instance->GetName()] = instance;
	return noDel_ptr<GameObject>(instance);
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z, float width, float height,
	noDel_ptr<Sprite> sprite, noDel_ptr<Transform> parent, std::string name)
{
	GameObject* instance = new GameObject(name);
	instance->SetSceneType(sceneType);
	//Transform�̍쐬
	instance->AddComponent<Transform>();
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	instance->transform->SetUpTransform(x, y, z, parent);
	//SpriteRenderer�̍쐬
	instance->AddComponent<SpriteRenderer>();
	instance->GetComponent<SpriteRenderer>()->SetUpRenderer(sprite);
	//�I�u�W�F�N�g�o�^
	umObjects[instance->GetName()] = instance;
	return noDel_ptr<GameObject>(instance);
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z,
	noDel_ptr<Mesh> mesh, noDel_ptr<Transform> parent, std::string name)
{
	GameObject* instance = new GameObject(name);
	instance->SetSceneType(sceneType);
	//Transform�̍쐬
	instance->AddComponent<Transform>();
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	instance->transform->SetUpTransform(x, y, z, parent);
	//SpriteRenderer�̍쐬
	instance->AddComponent<MeshRenderer>();
	instance->GetComponent<MeshRenderer>()->SetUpMeshRenderer(mesh);
	//�I�u�W�F�N�g�o�^
	umObjects[instance->GetName()] = instance;
	return noDel_ptr<GameObject>(instance);
}
noDel_ptr<GameObject> GameObjectManager::CreateImageObject(float x, float y, float width, float height,
	noDel_ptr<Sprite> sprite, noDel_ptr<Transform> parent, std::string name)
{
	GameObject* instance = new GameObject(name);
	instance->SetSceneType(sceneType);
	//Transform�̍쐬
	instance->AddComponent<Transform>();
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	instance->transform->SetUpTransform(x, y, 0, parent);
	//SpriteRenderer�̍쐬
	instance->AddComponent<ImageRenderer>();
	instance->GetComponent<ImageRenderer>()->SetUpRenderer2D(width, height, sprite);
	//�I�u�W�F�N�g�o�^
	umObjects[instance->GetName()] = instance;
	return noDel_ptr<GameObject>(instance);
}

//�I�u�W�F�N�g�̔j������
void GameObjectManager::ReserveDestroyObject(std::string name) {
	vDestroyName.emplace_back(name);
}
//�I�u�W�F�N�g�̗L������
void GameObjectManager::ReserveEnableObject(std::string name) {
	vEnableName.emplace_back(name);
}

noDel_ptr<GameObject> GameObjectManager::GetGameObject(std::string name) {
	return noDel_ptr<GameObject>(umObjects[name]);
}

//�z�񂩂����̗v�f�𔲂��o��
void GameObjectManager::PullOutComponent(noDel_ptr<GameObject> obj) {
	std::unordered_map<int, noDel_ptr<Component>>* umap = nullptr;
	bool isOnce = true;
	for (Component* com : obj->components) {
		if (com->type == eComponentType::Transform) umap = &umTransform;
		else if (com->type == eComponentType::Renderer2D) {
			for (int i = 0; i < v2DRenderer.size(); i++) {
				if (v2DRenderer[i]->GetInstanceID() == com->GetInstanceID()) {
					v2DRenderer.erase(v2DRenderer.begin() + i);
					isSortEnable = true;
					break;
				}
			}
			continue;
		}
		else if (com->type == eComponentType::Renderer3D) umap = &um3DRenderer;
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

//�R���|�[�l���g�̓o�^�Ɗe�평���ݒ�(�R���X�g���N�^���ɂ͓ǂݎ��Ȃ�gameObject���g���邽�߂����Őݒ�)
void GameObjectManager::RegistComponent(noDel_ptr<Component> com) {
	std::unordered_map<int, noDel_ptr<Component>>* umap = nullptr;

	if (com->IsRegisted()) return; //�o�^�ς݂Ȃ疳��

	com->SetRegistState(true); //�o�^�󋵂�ύX
	if (com->type == eComponentType::Transform) umap = &umTransform;
	else if (com->type == eComponentType::Renderer2D) {
		v2DRenderer.emplace_back(com);
		isSortEnable = true;
		return;
	}
	else if (com->type == eComponentType::Renderer3D) umap = &um3DRenderer;
	else if (com->type == eComponentType::Collider) umap = &umCollider2D;
	else if (com->type == eComponentType::Physics) umap = &umPhysics2D;
	else if (com->type == eComponentType::Behaviour) umap = &umBehaviour;
	else if (com->type == eComponentType::Animator) umap = &umAnimator;
	else return;

	(*umap)[com->GetInstanceID()] = com;

	//Behaviour�̏ꍇAwake���Ăяo��
	if (com->type == eComponentType::Behaviour) {
		com->Execute((int)eBehaviourState::Awake);
	}
}

//�R���|�[�l���g�L����������
bool GameObjectManager::CheckComponentEnable(noDel_ptr<Component> com) {
	//���̃t���[���ŗL���ɂȂ��Ă���ꍇ����
	bool _flag = false;
	for (auto& name : vEnableName) {
		if (com->gameObject->GetName() == name) return false;
	}
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
	int pivot = static_noDel_cast<Renderer2D>(v2DRenderer[left])->GetRenderPriority();

	while (true) {
		while (static_noDel_cast<Renderer2D>(v2DRenderer[left])->GetRenderPriority() < pivot) left++;
		while (static_noDel_cast<Renderer2D>(v2DRenderer[right])->GetRenderPriority() > pivot) right--;

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

	// �����ċA
	RenderOrderSort(start, left - 1);

	RenderOrderSort(right + 1, end);
}