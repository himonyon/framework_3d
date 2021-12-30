#pragma once
/*-----------------------------------------------------------

	Behaviour�N���X
		�I�u�W�F�N�g�̐U�镑���������N���X

-------------------------------------------------------------*/

enum class eBehaviourState {
	Update,
};

class Behaviour : public Component {
private:
	bool startCalled = false;

	noDel_ptr<Collider2D> c2d = 0;

public:
	Behaviour();
	virtual ~Behaviour() {};

	//�R���|�[�l���g����
	void Execute() override;
	void Execute(int state) override;

private:
	//�I�u�W�F�N�g�����ꂽ�Ƃ��ɌĂяo�����֐�
	virtual void Start() = 0;

	//���t���[���Ăяo����鏈���֐�
	virtual void Update() = 0;

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