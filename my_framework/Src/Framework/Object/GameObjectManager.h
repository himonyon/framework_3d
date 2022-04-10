#pragma once
/*-----------------------------------------------------------

	GameObjectManager�N���X(���N���X)
		�Q�[���I�u�W�F�N�g�̃I�u�W�F�N�g�v�[��
		�Q�[���I�u�W�F�N�g�̊Ǘ�

-------------------------------------------------------------*/

namespace MyFrameWork {
	class GameObjectManager {
	private:
		int sceneType = 0;

		bool isStartFucnEnable = false; //�X�^�[�g�֐������s���邩

		bool isSortEnable = false; //�X�v���C�g�̃\�[�g�����s���邩

	public:
		GameObjectManager() {};
		~GameObjectManager();

		//����
		void Execute();
		//�`��
		void Render();

		//�I�u�W�F�N�g�̍폜�\��
		void ReserveDestroyObject(std::string name);

		//�I�u�W�F�N�g�̗L���\��
		void ReserveEnableObject(std::string name);

		//�I�u�W�F�N�g�N���G�C�^
		//��I�u�W�F�N�g�쐬
		noDel_ptr<GameObject> CreateObject(float x, float y, float z,
			noDel_ptr<Transform> parent = nullptr, std::string name = "");
		//�X�v���C�g�I�u�W�F�N�g�쐬
		noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Sprite> sprite,
			noDel_ptr<Transform> parent = nullptr, std::string name = "");
		//���b�V���I�u�W�F�N�g�쐬
		noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Mesh> mesh,
			noDel_ptr<Transform> parent = nullptr, std::string name = "");
		//�C���[�W(UI)�I�u�W�F�N�g�쐬
		noDel_ptr<GameObject> CreateImageObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite,
			noDel_ptr<Transform> parent = nullptr, std::string name = "");

	private:
		//�z�񂩂�����ID�̃R���|�[�l���g�𔲂�
		void PullOutComponent(noDel_ptr<GameObject> obj);
		//Vector�^�̔z�񂩂�R���|�[�l���g�𔲂�
		void PullOutComponentToVector(std::vector<noDel_ptr<Component>>& renderer, noDel_ptr<Component> com);

		//�R���|�[�l���g�����̗L���������m�F����
		bool CheckComponentEnable(noDel_ptr<Component> com);

		//�`�揇�̕ύX(�N�C�b�N�\�[�g)
		void RenderOrderSort(std::vector<noDel_ptr<Component>>& renderer, int start, int end);

		//Vector�^�ɃR���|�[�l���g��o�^����
		void RegistComponentToVector(std::vector<noDel_ptr<Component>>& renderer, noDel_ptr<Component> com);

	public:
		//�I�u�W�F�N�g�ɒǉ����ꂽ�R���|�[�l���g�����̃N���X�̔z��Ɋi�[
		void RegistComponent(noDel_ptr<Component> com);

		//�Q�[���I�u�W�F�N�g���擾����
		noDel_ptr<GameObject> GetGameObject(std::string name);

		//getter,setter
		void SetSceneType(int val) { sceneType = val; }
		int GetSceneType() const { return sceneType; }
		void SetStartFuncEnable() { isStartFucnEnable = true; }
		void SetSortEnable() { isSortEnable = true; }

	private:
		//�Q�[���I�u�W�F�N�g�z��
		std::unordered_map<std::string, GameObject*> umObjects;

		//�R���|�[�l���g�̔z��
		std::unordered_map<int, noDel_ptr<Component>> umTransform;
		std::unordered_map<int, noDel_ptr<Component>> umCollider2D;
		std::unordered_map<int, noDel_ptr<Component>> umPhysics2D;
		std::vector<noDel_ptr<Component>> vImageRenderer;
		std::vector<noDel_ptr<Component>> vSpriteRenderer;
		std::vector<noDel_ptr<Component>> vMeshRenderer;
		std::unordered_map<int, noDel_ptr<Component>> umBehaviour;
		std::unordered_map<int, noDel_ptr<Component>> umAnimator;

		//�\�񂵂Ĉꊇ���삷�邽�߂̃R���e�i
		std::vector<std::string> vDestroyName; //�폜����I�u�W�F�N�g��
		std::vector<std::string> vEnableName; //�L���E��������I�u�W�F�N�g��(Behaviour�̑������s��h��)
	};
}