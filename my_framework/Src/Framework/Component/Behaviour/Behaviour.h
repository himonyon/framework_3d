#pragma once
/*-----------------------------------------------------------

	Behaviour�R���|�[�l���g
		�I�u�W�F�N�g�̐U�镑���������N���X

-------------------------------------------------------------*/

namespace MyFrameWork {

	//Behaviour�̎��s�^�C�v
	enum class eBehaviourState {
		Awake,
		Update,
	};

	class Behaviour : public Component {
	private:
		//�X�^�[�g�֐��̃t���O
		bool startCalled = false;

		//�R���C�_�[
		noDel_ptr<Collider2D> c2d = 0;

	public:
		Behaviour();
		virtual ~Behaviour() {};

		//�R���|�[�l���g����
		void Execute() override;
		void Execute(int state) override;

	private:
		//�I�u�W�F�N�g���쐬���ꂽ�Ƃ��ɌĂяo�����֐�
		virtual void Awake() {};

		//�ŏ��̃t���[���ŌĂяo�����֐�
		virtual void Start() {};

		//���t���[���Ăяo����鏈���֐�
		virtual void Update() {};

		//�I�u�W�F�N�g�̏Փˏ���
		virtual void OnCollision2D(noDel_ptr<Collider2D> hitCollider) {};
		virtual void OnCollisionEnter2D(noDel_ptr<Collider2D> hitCollider) {};
		virtual void OnCollisionExit2D(noDel_ptr<Collider2D> exitCollider) {};
		virtual void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) {};
		virtual void OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) {};
		virtual void OnTriggerExit2D(noDel_ptr<Collider2D> exitCollider) {};

	protected:
		noDel_ptr<Sprite> CreateSprite(Sprite* sprite);
	};
}