#pragma once
/*-----------------------------------------------------------

	�R���C�_�[2D�R���|�[�l���g
		�l�p�`�̓����蔻��

-------------------------------------------------------------*/

namespace MyFrameWork {
	class Collider2D : public Component {
	public:
		//�ڐG����
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

		//�������Ă���R���C�_�[���i�[
		void AddHitCollisions(noDel_ptr<Collider2D> hitColider);
		void AddHitTriggers(noDel_ptr<Collider2D> hitColider);

		//�������Ԃ̃N���A
		void ClearHitState();

	private:
		//�����蔻��
		void IsCollide(noDel_ptr<Collider2D> hitColider);
	};
}