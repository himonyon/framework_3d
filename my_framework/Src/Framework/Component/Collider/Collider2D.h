#pragma once
/*-----------------------------------------------------------

	�R���C�_�[2D�N���X
		2D�̎l�p�`�̓����蔻��

-------------------------------------------------------------*/

class Collider2D : public Component {
public:
	bool isCollision = true;
	float sizeX;
	float sizeY;

	//�Փ˂��Ă���R���C�_�[
	std::vector<noDel_ptr<Collider2D>> hitCollisions;
	std::vector<noDel_ptr<Collider2D>> hitTriggers;

public:
	Collider2D(bool collision = true);
	Collider2D(float width, float height, bool collision = true);
	~Collider2D() {};

	//�R���|�[�l���g����
	void Execute(noDel_ptr<Collider2D> hitCollider) override;

private:
	void IsCollide(noDel_ptr<Collider2D> hitColider);

	void AddHitCollisions(noDel_ptr<Collider2D> hitColider);
	void AddHitTriggers(noDel_ptr<Collider2D> hitColider);
};