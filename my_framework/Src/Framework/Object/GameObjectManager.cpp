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
	for (auto& ph : vPhysics2D) {
		if (!CheckComponentEnable(ph)) continue;
		noDel_ptr<Collider2D> col = ph->gameObject->GetComponent<Collider2D>();
		if(col == nullptr) continue;
		for (auto& c2d : vCollider2D) {
			if (!CheckComponentEnable(c2d)) continue;
			if (col == c2d) continue;
			col->Execute(c2d);
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
	bool _endSort = false; //�\�[�g�ς݂��`�F�b�N
	for (auto& sprite : vSpriteRenderer) {
		if (sprite->isSortSwitch()) {
			sprite->SetSortSwitch(false);
			if (!_endSort)RenderOrderSort(0, (int)vSpriteRenderer.size() - 1);
		}
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
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z, noDel_ptr<Transform> parent) {
	GameObject* instance = new GameObject();
	instance->SetSceneType(sceneType);
	//Transform�̍쐬
	instance->AddComponent<Transform>(new Transform(x, y, z, parent));
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	objects.emplace_back(instance);
	return noDel_ptr<GameObject>(objects.back());
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite, noDel_ptr<Transform> parent) {
	GameObject* instance = new GameObject();
	instance->SetSceneType(sceneType);
	//Transform�̍쐬
	instance->AddComponent<Transform>(new Transform(x, y, 0, parent));
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	//SpriteRenderer�̍쐬
	instance->AddComponent<SpriteRenderer>(new SpriteRenderer(width, height, sprite));
	objects.emplace_back(instance);
	return noDel_ptr<GameObject>(objects.back());
}
noDel_ptr<GameObject> GameObjectManager::CreateObject(float x, float y, float z, noDel_ptr<Mesh> mesh, noDel_ptr<Transform> parent) {
	GameObject* instance = new GameObject();
	instance->SetSceneType(sceneType);
	//Transform�̍쐬
	instance->AddComponent<Transform>(new Transform(x, y, z, parent));
	instance->transform = noDel_ptr<Transform>(instance->GetComponent<Transform>());
	//SpriteRenderer�̍쐬
	instance->AddComponent<MeshRenderer>(new MeshRenderer(mesh));
	objects.emplace_back(instance);
	return noDel_ptr<GameObject>(objects.back());
}

//�I�u�W�F�N�g�̔j������
void GameObjectManager::ReserveDestroyObject(int objID) {
	vDestroyID.emplace_back(objID);
}

//�z�񂩂����̗v�f�𔲂��o��
void GameObjectManager::PullOutComponent(noDel_ptr<GameObject> obj) {
	for (auto& com : obj->components) {
		if (com->type == eComponentType::Transform) PullOutTransform(noDel_ptr<Component>(com));
		if (com->type == eComponentType::Collider) PullOutCollider2D(noDel_ptr<Component>(com));
		if (com->type == eComponentType::Physics) PullOutPhysics2D(noDel_ptr<Component>(com));
		if (com->type == eComponentType::SpriteRenderer) PullOutSpriteRenderer(noDel_ptr<Component>(com));
		if (com->type == eComponentType::MeshRenderer) PullOutMeshRenderer(noDel_ptr<Component>(com));
		if (com->type == eComponentType::Behaviour) PullOutBehaviour(noDel_ptr<Component>(com));
		if (com->type == eComponentType::Animator) PullOutAnimator(noDel_ptr<Component>(com));
	}
}

void GameObjectManager::PullOutTransform(noDel_ptr<Component>com) {
	for (int i = 0; i < vTransform.size(); i++) {
		if (com->GetObjectID() == vTransform[i]->GetObjectID()) {
			vTransform.erase(vTransform.begin() + i);
			return;
		}
	}
}
void GameObjectManager::PullOutCollider2D(noDel_ptr<Component> com) {
	for (int i = 0; i < vCollider2D.size(); i++) {
		if (com->GetObjectID() == vCollider2D[i]->GetObjectID()) {
			vCollider2D.erase(vCollider2D.begin() + i);
			return;
		}
	}
}
void GameObjectManager::PullOutPhysics2D(noDel_ptr<Component> com) {
	for (int i = 0; i < vPhysics2D.size(); i++) {
		if (com->GetObjectID() == vPhysics2D[i]->GetObjectID()) {
			vPhysics2D.erase(vPhysics2D.begin() + i);
			return;
		}
	}
}
void GameObjectManager::PullOutSpriteRenderer(noDel_ptr<Component> com) {
	for (int i = 0; i < vSpriteRenderer.size(); i++) {
		if (com->GetObjectID() == vSpriteRenderer[i]->GetObjectID()) {
			vSpriteRenderer.erase(vSpriteRenderer.begin() + i);
			return;
		}
	}
}
void GameObjectManager::PullOutMeshRenderer(noDel_ptr<Component> com) {
	for (int i = 0; i < vMeshRenderer.size(); i++) {
		if (com->GetObjectID() == vMeshRenderer[i]->GetObjectID()) {
			vMeshRenderer.erase(vMeshRenderer.begin() + i);
			return;
		}
	}
}
void GameObjectManager::PullOutBehaviour(noDel_ptr<Component> com) {
	for (int i = 0; i < vBehaviour.size(); i++) {
		if (com->GetObjectID() == vBehaviour[i]->GetObjectID()) {
			vBehaviour.erase(vBehaviour.begin() + i);
			--i; //�������݂���\�������邽�ߍŌ�܂Ń��[�v����
		}
	}
}
void GameObjectManager::PullOutAnimator(noDel_ptr<Component> com) {
	for (int i = 0; i < vAnimator.size(); i++) {
		if (com->GetObjectID() == vAnimator[i]->GetObjectID()) {
			vAnimator.erase(vAnimator.begin() + i);
			return;
		}
	}
}



//�R���|�[�l���g�̓o�^�Ɗe�평���ݒ�(�R���X�g���N�^���ɂ͓ǂݎ��Ȃ�gameObject���g���邽�߂����Őݒ�)
void GameObjectManager::RegistComponent(noDel_ptr<GameObject> obj) {
	for (Component* com : obj->components) {
		if (com->IsRegisted()) continue;
		//�ǉ�����
		com->SetRegistState(true);
		if (AddTransform(noDel_ptr<Component>(com))) continue;
		if (AddSpriteRenderer(noDel_ptr<Component>(com))) continue;
		if (AddMeshRenderer(noDel_ptr<Component>(com))) continue;
		if (AddCollider2D(noDel_ptr<Component>(com))) continue;
		if (AddBehaviour(noDel_ptr<Component>(com))) continue;
		if (AddPhysics2D(noDel_ptr<Component>(com))) continue;
		if (AddAnimator(noDel_ptr<Component>(com))) continue;
	}
	obj->isAddNewComponent = false;
}

bool GameObjectManager::AddTransform(noDel_ptr<Component> com) {
	noDel_ptr<Transform> component = DownCastComponent<Transform>(com);
	if (component != nullptr) {
		vTransform.emplace_back(component);
		return true;
	}
	return false;
}
bool GameObjectManager::AddCollider2D(noDel_ptr<Component>com) {
	noDel_ptr<Collider2D> component = DownCastComponent<Collider2D>(com);
	if (component != nullptr) {
		vCollider2D.emplace_back(component);
		if (component->gameObject->GetComponent<SpriteRenderer>() != nullptr) {
			///�X�v���C�g�����݂���Ƃ��ɃT�C�Y�������O�i���ݒ�j�Ȃ�摜�̃T�C�Y���R���C�_�[�ɐݒ肷��
			if (component->sizeX == 0 && component->sizeY == 0) {
				component->sizeX = component->gameObject->GetComponent<SpriteRenderer>()->sizeX;
				component->sizeY = component->gameObject->GetComponent<SpriteRenderer>()->sizeY;
			}
		}
		return true;
	}
	return false;
}
bool GameObjectManager::AddPhysics2D(noDel_ptr<Component>com) {
	noDel_ptr<Physics2D> component = DownCastComponent<Physics2D>(com);
	if (component != nullptr) {
		vPhysics2D.emplace_back(component);
		return true;
	}
	return false;
}
bool GameObjectManager::AddSpriteRenderer(noDel_ptr<Component> com) {
	noDel_ptr<SpriteRenderer> component = DownCastComponent<SpriteRenderer>(com);
	if (component != nullptr) {
		vSpriteRenderer.emplace_back(component);
		return true;
	}
	return false;
}
bool GameObjectManager::AddMeshRenderer(noDel_ptr<Component> com) {
	noDel_ptr<MeshRenderer> component = DownCastComponent<MeshRenderer>(com);
	if (component != nullptr) {
		vMeshRenderer.emplace_back(component);
		return true;
	}
	return false;
}
bool GameObjectManager::AddBehaviour(noDel_ptr<Component> com) {
	noDel_ptr<Behaviour>  component = DownCastComponent<Behaviour>(com);
	if (component != nullptr) {
		vBehaviour.emplace_back(component);
		isStartFucnEnable = true;
		MessageSystem::SendMessageToCom(component, L"Start");
		return true;
	}
	return false;
}
bool GameObjectManager::AddAnimator(noDel_ptr<Component> com) {
	noDel_ptr<Animator> component = DownCastComponent<Animator>(com);
	if (component != nullptr) {
		vAnimator.emplace_back(component);
		return true;
	}
	return false;
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

	int pivot = vSpriteRenderer[left]->GetRenderPriority();

	while (true) {
		while (vSpriteRenderer[left] == NULL) left++;
		while (vSpriteRenderer[right] == NULL) right--;
		while (vSpriteRenderer[left]->GetRenderPriority() < pivot) left++;
		while (vSpriteRenderer[right]->GetRenderPriority() > pivot) right--;

		if (left < right) {
			noDel_ptr<SpriteRenderer> temp = vSpriteRenderer[left];
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