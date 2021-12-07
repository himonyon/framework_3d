#pragma once
/*-----------------------------------------------------------

	�Q�[���I�u�W�F�N�g�N���X
		�Q�[���I�u�W�F�N�g�̊Ǘ�

-------------------------------------------------------------*/

class SceneManager;

class GameObject : public Object {
public:
	//���W�A��]�A�X�P�[���ϊ��\����
	noDel_ptr<Transform> transform = 0;

	//�R���|�[�l���g���V���ɒǉ����ꂽ��
	bool isAddNewComponent = false;

	//�o�^���ꂽ�R���|�[�l���g
	std::vector<Component*> components;

private:
	bool objEnable; //���s�Ǘ�
	//�I�u�W�F�N�g�̏����V�[��(eSceneTable)
	int belongSceneType = 0;

public:
	GameObject();
	~GameObject(void); 

	//�I�u�W�F�N�g�̔j��
	void Destroy(noDel_ptr<GameObject> obj = nullptr);

	//getter, setter
	void SetObjEnable(bool flag); //���s��Ԃ̐ݒ�
	bool IsObjEnable(); //���s��ԂɂȂ��Ă��邩�ǂ���

	//�R���|�[�l���g�̒ǉ�
	template<class T>
	void AddComponent() {
		T* instance = new T();
		instance->gameObject = noDel_ptr<GameObject>(this);
		if(transform != nullptr) instance->transform = transform;
		components.emplace_back(instance);
		isAddNewComponent = true;
		SceneManager::GetScene(belongSceneType)->SetAddComponentTrigger();
	}

	//�R���|�[�l���g�̎擾
	template<class T>
	noDel_ptr<T> GetComponent() {
		T* component;
		for (Component* com : components) {
			component = dynamic_cast<T*>(com);
			if (component != nullptr) {
				return noDel_ptr<T>(component);
			}
		}

		return nullptr;
	}

	void SetSceneType(int value) { belongSceneType = value; }
	int GetSceneType() { return belongSceneType; }
};
