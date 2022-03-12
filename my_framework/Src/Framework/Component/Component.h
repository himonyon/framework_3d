#pragma once
/*-----------------------------------------------------------

	�R���|�[�l���g�N���X(���N���X)
		�I�u�W�F�N�g�Ɗe��@�\�N���X�����т���N���X

-------------------------------------------------------------*/
namespace MyFrameWork {

	class GameObject;
	class Transform;
	class Collider2D;

	enum class eComponentType {
		None,
		Transform,
		Behaviour,
		Renderer3D,
		Renderer2D,
		Collider,
		Physics,
		Animator,
		Camera,
		Sound,
	};

	class Component : public Object, public MessageSystem {
	private:
		bool isRegisted = false;

		bool isEnable = true;

	public:
		noDel_ptr<GameObject> gameObject = 0;

		noDel_ptr<Transform> transform = 0;

		eComponentType type = eComponentType::None;

	public:
		Component(eComponentType type);
		virtual ~Component() {};

		//�R���|�[�l���g�̊e�������s�֐�
		virtual void Execute() {};
		virtual void Execute(int state) {};
		virtual void Execute(noDel_ptr<Collider2D> col) {};

		//�g�p�\��
		bool IsEnable() { return isEnable; }

		//�I�u�W�F�N�g�}�l�[�W���[�֓o�^�ς݂�
		bool IsRegisted() { return isRegisted; }

		//Getter,Setter
		void SetRegistState(bool flag);
		void SetEnable(bool flag);
	};
}
