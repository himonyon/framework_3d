#pragma once
/*-----------------------------------------------------------

	�R���C�_�[2D�N���X
		2D�̎l�p�`�̓����蔻��

-------------------------------------------------------------*/

class Collider2D : public Component {
public:
	bool isCollision = true;
	float sizeX = 0;
	float sizeY = 0;

	//�Փ˂��Ă���R���C�_�[
	std::vector<noDel_ptr<Collider2D>> hitCollisions;
	std::vector<noDel_ptr<Collider2D>> hitTriggers;

	//�O�t���[���ŏՓ˂����R���C�_�[
	std::vector<noDel_ptr<Collider2D>> b_hitCollisions; 
	std::vector<noDel_ptr<Collider2D>> b_hitTriggers;

public:
	Collider2D();
	~Collider2D();

	//�R���|�[�l���g�̏�����
	void SetUpCollider2D(bool collision);
	void SetUpCollider2D(float sizeX, float sizeY, bool collision);

	//�R���|�[�l���g����
	void Execute();
	void Execute(noDel_ptr<Collider2D> hitCollider) override;
	void AddHitCollisions(noDel_ptr<Collider2D> hitColider);
	void AddHitTriggers(noDel_ptr<Collider2D> hitColider);

	//�������Ԃ̃N���A
	void ClearHitState();

private:
	void IsCollide(noDel_ptr<Collider2D> hitColider);
};