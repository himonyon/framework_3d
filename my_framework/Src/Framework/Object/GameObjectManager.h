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
	bool isSortEnable = false; //�X�v���C�g�̃\�[�g�����s���邩

public:
	GameObjectManager() {};
	~GameObjectManager();

	void Execute();
	void Render();

	//�I�u�W�F�N�g�̍폜�\��
	void ReserveDestroyObject(int objID);

	//�I�u�W�F�N�g�N���G�C�^
	//��I�u�W�F�N�g�쐬
	noDel_ptr<GameObject> CreateObject(float x, float y, float z,
		noDel_ptr<Transform> parent = nullptr, bool local = false);
	//�X�v���C�g�I�u�W�F�N�g�쐬
	noDel_ptr<GameObject> CreateObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite,
		noDel_ptr<Transform> parent = nullptr, bool local = false);
	//���b�V���I�u�W�F�N�g�쐬
	noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Mesh> mesh, 
		noDel_ptr<Transform> parent = nullptr, bool local = false);

private:
	//�z�񂩂�����ID�̃R���|�[�l���g�𔲂�
	void PullOutComponent(noDel_ptr<GameObject> obj);

	//�R���|�[�l���g�����̗L���������m�F����
	bool CheckComponentEnable(noDel_ptr<Component> com);

	//�`�揇�̕ύX(�N�C�b�N�\�[�g)
	void RenderOrderSort(int start, int end);
	
	//�I�u�W�F�N�g�ɒǉ����ꂽ�R���|�[�l���g�����̃N���X�̔z��Ɋi�[
	void RegistComponent(noDel_ptr<GameObject> obj);


public:
	//getter,setter
	void SetSceneType(int val) { sceneType = val; }
	void SetAddComponentTrigger() { isAddComponent = true; }
	void SetStartFuncEnable() { isStartFucnEnable = true; }
	void SetSortEnable() { isSortEnable = true; }

private:
	std::unordered_map<int, GameObject*> umObjects;

	//�R���|�[�l���g�̔z��
	std::unordered_map<int,noDel_ptr<Component>> umTransform;
	std::unordered_map<int,noDel_ptr<Component>> umCollider2D;
	std::unordered_map<int,noDel_ptr<Component>> umPhysics2D;
	std::vector<noDel_ptr<Component>> v2DRenderer;
	std::unordered_map<int,noDel_ptr<Component>> umMeshRenderer;
	std::unordered_map<int,noDel_ptr<Component>> umBehaviour;
	std::unordered_map<int,noDel_ptr<Component>> umAnimator;

public:
	std::vector<int> vDestroyID; //�폜����I�u�W�F�N�gID
};
