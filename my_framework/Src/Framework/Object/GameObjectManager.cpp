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
		RenderOrderSort(vImageRenderer ,0, (int)vImageRenderer.size() - 1);
		RenderOrderSort(vSpriteRenderer ,0, (int)vSpriteRenderer.size() - 1);
		RenderOrderSort(vMeshRenderer ,0, (int)vMeshRenderer.size() - 1);
		isSortEnable = false;
	}
}

void GameObjectManager::Render() {
	///�w��UI�I�u�W�F�N�g�̕`��
	for (auto& image : vImageRenderer) {
		if (image->gameObject->IsFrontObj()) continue;
		if (!CheckComponentEnable(image)) continue;
		image->Execute();
	}

	//�X�v���C�g�̕`��
	for (auto& sprite : vSpriteRenderer) {
		if (!CheckComponentEnable(sprite)) continue;
		sprite->Execute();
	}

	//���b�V���̕`��
	for (auto& mesh : vMeshRenderer) {
		if (!CheckComponentEnable(mesh)) continue;
		mesh->Execute();
	}

	///�O��UI�I�u�W�F�N�g�̕`��
	for (auto& image : vImageRenderer) {
		if (image->gameObject->IsFrontObj() == false) continue;
		if (!CheckComponentEnable(image)) continue;
		image->Execute();
	}
}

//�I�u�W�F�N�g�̍쐬
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z, noDel_ptr<Transform> parent, std::string name) {
	GameObject* _instance = new GameObject(name);
	_instance->SetSceneType(sceneType);
	//Transform�̍쐬
	_instance->AddComponent<Transform>();
	_instance->transform = noDel_ptr<Transform>(_instance->GetComponent<Transform>());
	_instance->transform->SetUpTransform(x, y, z, parent);
	//�I�u�W�F�N�g�o�^
	umObjects[_instance->GetName()] = _instance;
	return noDel_ptr<GameObject>(_instance);
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z, noDel_ptr<Sprite> sprite, noDel_ptr<Transform> parent, std::string name)
{
	GameObject* _instance = new GameObject(name);
	_instance->SetSceneType(sceneType);
	//Transform�̍쐬
	_instance->AddComponent<Transform>();
	_instance->transform = noDel_ptr<Transform>(_instance->GetComponent<Transform>());
	_instance->transform->SetUpTransform(x, y, z, parent);
	//SpriteRenderer�̍쐬
	_instance->AddComponent<SpriteRenderer>();
	_instance->GetComponent<SpriteRenderer>()->SetUpSpriteRenderer(sprite);
	//�I�u�W�F�N�g�o�^
	umObjects[_instance->GetName()] = _instance;
	return noDel_ptr<GameObject>(_instance);
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z,
	noDel_ptr<Model> model, noDel_ptr<Transform> parent, std::string name)
{
	int _createCount = 0; //�����J�E���g

	//��̃I�u�W�F�N�g���쐬���Đe�ɂ���
	noDel_ptr<GameObject> _rootObj = CreateObject(x, y, z, parent, name);

	//���O���Ȃ��ꍇ�A��I�u�W�F�N�g�̖��O��ݒ�
	if (name == "") name = _rootObj->GetName();

	noDel_ptr<GameObject> _parentObj = _rootObj; //�e�I�u�W�F�N�g

	//���[�g���b�V���Ƃ��̎q���b�V���S�ăI�u�W�F�N�g������
	int _rootNum = model->GetRootCount();
	//���[�g���b�V���擾
	for (int i = 0; i < _rootNum; i++) {
		noDel_ptr<Mesh> _mesh = model->GetRootMesh(i);
		if (_mesh == NULL) continue; //�f�[�^���Ȃ���Ώ������Ȃ�
		//���[�g���b�V���̃I�u�W�F�N�g����
		std::string _newName = name + "_" + std::to_string(_createCount); //�V�������O
		//���W
		stVector3 _newPos = _mesh->GetInitPos();
		stVector3 _newRot = _mesh->GetInitRot();
		stVector3 _newScl = _mesh->GetInitScl();
		//����
		noDel_ptr<GameObject> _newObj = CreateObject(_newPos, _newRot, _newScl, _mesh, _parentObj->transform, _newName);
		_parentObj = _newObj;
		_createCount++;
		//�q���b�V���̐���
		CreateObjectForChildByRootMesh(_mesh, _parentObj->transform, name, _createCount);

		_parentObj = _rootObj; //�e�I�u�W�F�N�g���Đݒ�
	}

	return _rootObj;
}
void GameObjectManager::CreateObjectForChildByRootMesh(noDel_ptr<Mesh> mesh,
	noDel_ptr<Transform> parent, std::string name, int& createCount) {
	//�q���b�V�����I�u�W�F�N�g��
	int _childCount = mesh->GetChildCount();
	for (int i = 0; i < _childCount; i++) {
		noDel_ptr<Mesh> _child = mesh->GetChild(i);
		if (_child == NULL) continue; //�f�[�^���Ȃ���Ώ������Ȃ�
		//���b�V���I�u�W�F�N�g����
		std::string _newName = name + "_" + std::to_string(createCount); //�V�������O
		stVector3 _newPos = _child->GetInitPos();
		stVector3 _newRot = _child->GetInitRot();
		stVector3 _newScl = _child->GetInitScl();
		noDel_ptr<GameObject> _newObj = CreateObject(_newPos, _newRot, _newScl, _child, parent, _newName);
		noDel_ptr<GameObject> _parentObj = _newObj; //�e�I�u�W�F�N�g
		createCount++; //�����J�E���gUP
		CreateObjectForChildByRootMesh(_child, _parentObj->transform, name, createCount);
	}
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(stVector3& pos, stVector3& rot, stVector3& scl,
	noDel_ptr<Mesh> mesh, noDel_ptr<Transform> parent, std::string name)
{
	GameObject* _instance = new GameObject(name);
	_instance->SetSceneType(sceneType);
	//Transform�̍쐬
	_instance->AddComponent<Transform>();
	_instance->transform = noDel_ptr<Transform>(_instance->GetComponent<Transform>());
	_instance->transform->SetUpTransform(pos, rot, scl, parent);
	//MeshRenderer�̍쐬
	_instance->AddComponent<MeshRenderer>();
	_instance->GetComponent<MeshRenderer>()->SetUpMeshRenderer(mesh);
	//�I�u�W�F�N�g�o�^
	umObjects[_instance->GetName()] = _instance;
	return noDel_ptr<GameObject>(_instance);
}

noDel_ptr<GameObject> GameObjectManager::CreateImageObject(float x, float y, float width, float height,
	noDel_ptr<Sprite> sprite, noDel_ptr<Transform> parent, std::string name)
{
	GameObject* _instance = new GameObject(name);
	_instance->SetSceneType(sceneType);
	//Transform�̍쐬
	_instance->AddComponent<Transform>();
	_instance->transform = noDel_ptr<Transform>(_instance->GetComponent<Transform>());
	_instance->transform->SetUpTransform(x, y, 0, parent);
	//SpriteRenderer�̍쐬
	_instance->AddComponent<ImageRenderer>();
	_instance->GetComponent<ImageRenderer>()->SetUpImageRenderer(width, height, sprite);
	//�I�u�W�F�N�g�o�^
	umObjects[_instance->GetName()] = _instance;
	return noDel_ptr<GameObject>(_instance);
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
	bool isOnce = true; //��������R���|�[�l���g���Ȃ��ꍇ��TRUE
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

//�R���|�[�l���g�̓o�^�Ɗe�평���ݒ�(�R���X�g���N�^���ɂ͓ǂݎ��Ȃ�gameObject���g���邽�߂����Őݒ�)
void GameObjectManager::RegistComponent(noDel_ptr<Component> com) {
	std::unordered_map<int, noDel_ptr<Component>>* umap = nullptr;

	if (com->IsRegisted()) return; //�o�^�ς݂Ȃ疳��

	com->SetRegistState(true); //�o�^�󋵂�ύX
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

	//Behaviour�̏ꍇAwake���Ăяo��
	if (com->type == eComponentType::Behaviour) {
		com->Execute((int)eBehaviourState::Awake);
	}
}

void GameObjectManager::RegistComponentToVector(std::vector<noDel_ptr<Component>>& renderer, noDel_ptr<Component> com) {
	renderer.emplace_back(com);
	isSortEnable = true;
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
void GameObjectManager::RenderOrderSort(std::vector<noDel_ptr<Component>>& renderer, int start, int end) {
	int left = start;
	int right = end;
	if (left >= right) return;

	//�������C�ɂ���StaticCast�ɂ��Ă���
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

	// �����ċA
	RenderOrderSort(renderer, start, left - 1);

	RenderOrderSort(renderer, right + 1, end);
}