#include "../../../../framework.h"
#include "../../../../environment.h"


Collider2D::Collider2D(bool collision) : Component(eComponentType::Collider) {
	sizeX = 0;
	sizeY = 0;
	isCollision = collision;
}
Collider2D::Collider2D(float width, float height,  bool collision) : Component(eComponentType::Collider) {
	sizeX = width;
	sizeY = height;
	isCollision = collision;
}

void Collider2D::Execute(noDel_ptr<Collider2D> hitCollider) {
	for (auto& m : messages) {
		if (m == L"Clear") {
			hitCollisions.clear();
			hitTriggers.clear();
			messages.clear();
		}
	}

	IsCollide(hitCollider);
}

//�R���C�_�[���m�̏Փ˂����o����
void Collider2D::IsCollide(noDel_ptr<Collider2D> hitCollider)
{
	//�����̃G���[�`�F�b�N
	if (hitCollider == NULL)
	{
		return;
	}

	stVector3 hitPos = hitCollider->transform->position;
	stVector3 hitRot = hitCollider->transform->rotation;
	stVector3 hitScl = hitCollider->transform->scale;

	//�߂�l�p�̕ϐ�
	bool hit = false;

	//�Փ˂̌��o
	float rx = 0.0f;	//�d�Ȃ��Ă��钷���`(Rectangle)�̕�
	float ry = 0.0f;	//�d�Ȃ��Ă��钷���`(Rectangle)�̍���

	//��(X)����

	//�X�v���C�g�̒��S���m�̍��W�̍���(differencial)
	float dx = fabsf(transform->position.x - hitPos.x);
	//2�̃X�v���C�g�̕�(����)���X�P�[���̍��v
	float sx = sizeX * 0.5f * transform->scale.x + hitCollider->sizeX * 0.5f * hitScl.x;

	if (dx < sx)	//����������菬������Ή�(X)�����œ������Ă���
	{
		//�c(Y)����

		//�X�v���C�g�̒��S���m�̍��W�̍���(differencial)
		float dy = fabsf(transform->position.y - hitPos.y);
		//2�̃X�v���C�g�̍���(����)���X�P�[���̍��v
		float sy = sizeY * 0.5f * transform->scale.y + hitCollider->sizeY * 0.5f * hitScl.y;

		if (dy < sy)	//������������菬������Ώc(Y)�����œ������Ă���
		{
			//�������Ă��Ȃ��Ƃ�����񂩂瓖�������Ƃ������ɏ���������
			hit = true;

			//�d�Ȃ��Ă��钷���`�̃T�C�Y��ێ�
			rx = sx - dx;
			ry = sy - dy;
		}
	}


	if (hit == false) return;

	//�Փ˂�����ꍇ�A�߂荞�݂���������
	//rigid�������Ă�����𓮂���
	if (isCollision && hitCollider->isCollision)
	{
		if (rx < ry)
		{	//�d�Ȃ����̈�(�����`)���c�������E����Փ�
			if (transform->position.x < hitCollider->transform->position.x)
			{	//p1��������Փ�
				transform->position.x -= rx;
			}
			else
			{	//p1���E����Փ�
				transform->position.x += rx;
			}
		}
		else
		{	//�d�Ȃ����̈�(�����`)���������㉺����Փ�
			if (transform->position.y < hitCollider->transform->position.y)
			{	//p1���ォ��Փ�
				transform->position.y -= ry;
			}
			else
			{	//p1��������Փ�
				transform->position.y += ry;
			}
		}
	}

	//�q�b�g�I�u�W�F�N�g�z��Ɋi�[
	std::wstring mes;
	bool _collision = false;
	if (isCollision && hitCollider->isCollision) {
		_collision = true;
		mes = L"OnCollision2D";
	}
	else {
		_collision = false;
		mes = L"OnTrigger2D";
	}
	//�I�u�W�F�N�g�Ƀ��b�Z�[�W���M
	for (auto& com : gameObject->components) {
		if (com->type == eComponentType::Behaviour && com->IsEnable()) {
			if(_collision) AddHitCollisions(hitCollider);
			else AddHitTriggers(hitCollider);
			MessageSystem::SendMessageToCom(noDel_ptr<Component>(com), mes);
		}
	}
	for (auto& com : hitCollider->gameObject->components) {
		if (com->type == eComponentType::Behaviour && com->IsEnable()) {
			if (_collision) hitCollider->AddHitCollisions(noDel_ptr<Collider2D>(this));
			else hitCollider->AddHitTriggers(noDel_ptr<Collider2D>(this));
			MessageSystem::SendMessageToCom(noDel_ptr<Component>(com), mes);
		}
	}
}

void Collider2D::AddHitCollisions(noDel_ptr<Collider2D> hitColider) {
	for (auto& v : hitCollisions) {
		if (v == hitColider) return;
	}
	hitCollisions.emplace_back(hitColider);
}
void Collider2D::AddHitTriggers(noDel_ptr<Collider2D> hitColider) {
	for (auto& v : hitTriggers) {
		if (v == hitColider) return;
	}
	hitTriggers.emplace_back(hitColider);
}