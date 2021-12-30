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

	//�o�^���ꂽ�R���|�[�l���g
	std::vector<Component*> components;

private:
	//�Q�[���I�u�W�F�N�g��
	std::string name;
	//���s�Ǘ�
	bool objEnable;
	//�I�u�W�F�N�g�̏����V�[��(eSceneTable)
	int belongSceneType = 0;

	//�ÓI�ȃI�u�W�F�N�g���ǂ���
	bool isStaticObj = false;

public:
	GameObject(std::string name);
	~GameObject(void);

	//�I�u�W�F�N�g�̔j��
	void Destroy(noDel_ptr<GameObject> obj = nullptr);

	//�I�u�W�F�N�g�̍쐬
	//��I�u�W�F�N�g�쐬
	noDel_ptr<GameObject> CreateObject(float x, float y, float z,
		noDel_ptr<Transform> parent = nullptr, std::string name = "");
	//�X�v���C�g�I�u�W�F�N�g�쐬
	noDel_ptr<GameObject> CreateObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite,
		noDel_ptr<Transform> parent = nullptr, std::string name = "");
	//���b�V���I�u�W�F�N�g�쐬
	noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Mesh> mesh,
		noDel_ptr<Transform> parent = nullptr, std::string name = "");

	//�Q�[���I�u�W�F�N�g�̌���
	noDel_ptr<GameObject> FindGameObject(std::string name);

	//�R���|�[�l���g�̒ǉ�
	template<class T>
	void AddComponent() {
		T* instance = new T();
		instance->gameObject = noDel_ptr<GameObject>(this);
		if (transform != nullptr) instance->transform = transform;
		components.emplace_back(instance);
		SceneManager::GetScene(belongSceneType)->RegistComponent(noDel_ptr<Component>(instance));
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

	//getter, setter
	void SetObjEnable(bool flag); //���s��Ԃ̐ݒ�
	bool IsObjEnable(); //���s��ԂɂȂ��Ă��邩�ǂ���

	void SetSceneType(int value) { belongSceneType = value; }
	int GetSceneType() { return belongSceneType; }

	void SetObjStatic(bool flag) { isStaticObj = flag; }
	bool IsObjStatic() { return isStaticObj; }

	std::string GetName() const { return name; }
};
