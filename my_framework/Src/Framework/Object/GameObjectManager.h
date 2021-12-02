#pragma once
/*-----------------------------------------------------------

	GameObjectManager�N���X(���N���X)
		�Q�[���I�u�W�F�N�g�̃I�u�W�F�N�g�v�[��
		�Q�[���I�u�W�F�N�g�̊Ǘ�

-------------------------------------------------------------*/

class GameObjectManager {
private:
	int sceneType = 0;

	bool isAddComponent = false; //�V���ɃR���|�[�l���g��ǉ�������
	bool isStartFucnEnable = false; //�X�^�[�g�֐������s���邩

public:
	GameObjectManager() {};
	~GameObjectManager();

	void Execute();
	void Render();

	//�I�u�W�F�N�g�̍폜�\��
	void ReserveDestroyObject(int objID);

	//�I�u�W�F�N�g�N���G�C�^�[
	noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Transform> parent = nullptr);
	noDel_ptr<GameObject> CreateObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite, noDel_ptr<Transform> parent = nullptr);
	noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Mesh> mesh, noDel_ptr<Transform> parent = nullptr);

private:
	//�z�񂩂�����ID�̃R���|�[�l���g�𔲂�
	void PullOutComponent(noDel_ptr<GameObject> obj);
	void PullOutTransform(noDel_ptr<Component> com);
	void PullOutCollider2D(noDel_ptr<Component> com);
	void PullOutPhysics2D(noDel_ptr<Component> com);
	void PullOutSpriteRenderer(noDel_ptr<Component> com);
	void PullOutMeshRenderer(noDel_ptr<Component> com);
	void PullOutBehaviour(noDel_ptr<Component> com);
	void PullOutAnimator(noDel_ptr<Component> com);

	//�R���|�[�l���g�����̗L���������m�F����
	bool CheckComponentEnable(noDel_ptr<Component> com);

	//�`�揇�̕ύX(�N�C�b�N�\�[�g)
	void RenderOrderSort(int start, int end);
	
	//�I�u�W�F�N�g�ɒǉ����ꂽ�R���|�[�l���g�����̃N���X�̔z��Ɋi�[
	void RegistComponent(noDel_ptr<GameObject> obj);

	//�R���|�[�l���g���_�E���L���X�g�������ʂ�Ԃ�
	template <class T>
	noDel_ptr<T> DownCastComponent(noDel_ptr<Component> com) {
		noDel_ptr<T> component = dynamic_noDel_cast<T>(com);
		if (component != nullptr) {
			return component;
		}
		return nullptr;
	}

	//�R���|�[�l���g��V���ɔz��ɉ�����
	bool AddTransform(noDel_ptr<Component> com);
	bool AddCollider2D(noDel_ptr<Component> com);
	bool AddSpriteRenderer(noDel_ptr<Component> com);
	bool AddMeshRenderer(noDel_ptr<Component> com);
	bool AddPhysics2D(noDel_ptr<Component> com);
	bool AddBehaviour(noDel_ptr<Component> com);
	bool AddAnimator(noDel_ptr<Component> com);

public:
	//getter,setter
	void SetSceneType(int val) { sceneType = val; }
	void SetAddComponentTrigger() { isAddComponent = true; }
	void SetStartFuncEnable() { isStartFucnEnable = true; }

private:
	std::vector<GameObject*> objects;

	//�R���|�[�l���g�̔z��
	std::vector<noDel_ptr<Transform>> vTransform;
	std::vector<noDel_ptr<Collider2D>> vCollider2D;
	std::vector<noDel_ptr<Physics2D>> vPhysics2D;
	std::vector<noDel_ptr<SpriteRenderer>> vSpriteRenderer;
	std::vector<noDel_ptr<MeshRenderer>> vMeshRenderer;
	std::vector<noDel_ptr<Behaviour>> vBehaviour;
	std::vector<noDel_ptr<Animator>> vAnimator;

public:
	std::vector<int> vDestroyID; //�폜����I�u�W�F�N�gID
};
