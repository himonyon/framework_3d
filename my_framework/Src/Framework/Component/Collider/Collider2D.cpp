#include "../../../../framework.h"
#include "../../../../environment.h"


Collider2D::Collider2D() : Component(eComponentType::Collider) {
	sizeX = 0;
	sizeY = 0;
	isCollision = true;
}

Collider2D::~Collider2D() {
	//�q�b�g���ɂ��鑊��̃R���C�_�[�̃q�b�g��񂩂玩�g�𔲂�
	for (auto& myHit : hitCollisions) {
		for (int i = 0; i < myHit->hitCollisions.size(); i++) {
			if (myHit->hitCollisions[i].get() == this) {
				myHit->hitCollisions.erase(myHit->hitCollisions.begin() + i);
				break;
			}
		}
	}

	for (auto& myHit : hitTriggers) {
		for (int i = 0; i < myHit->hitTriggers.size(); i++) {
			if (myHit->hitTriggers[i].get() == this) {
				myHit->hitTriggers.erase(myHit->hitTriggers.begin() + i);
				break;
			}
		}
	}
}

void Collider2D::SetUpCollider2D(bool collision) {
	noDel_ptr<SpriteRenderer> sr = gameObject->GetComponent<SpriteRenderer>();
	if (sr != nullptr) {
		sizeX = sr->sizeX;
		sizeY = sr->sizeY;
	}
	else {
		sizeX = 0;
		sizeY = 0;
	}
	this->isCollision = collision;
}
void Collider2D::SetUpCollider2D(float sizeX, float sizeY, bool collision) {
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->isCollision = collision;
}

void Collider2D::Execute() {
	ClearHitState();
}
void Collider2D::Execute(noDel_ptr<Collider2D> hitCollider) {
	//�����蔻��`�F�b�N
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
		mes = L"Collision";
	}
	else {
		_collision = false;
		mes = L"Trigger";
	}

	//�I�u�W�F�N�g�Ƀ��b�Z�[�W���M
	for (auto& com : gameObject->components) {
		if (com->type == eComponentType::Behaviour && com->IsEnable()) {
			if (_collision) AddHitCollisions(hitCollider);
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

void Collider2D::ClearHitState() {
	//�O�t���[���ł̏Փ˂����Z�b�g&beforeCollsion�̍X�V
	for (auto& m : messages) {
		if (m == L"Clear") {
			b_hitCollisions.clear();
			b_hitTriggers.clear();

			b_hitCollisions.resize(hitCollisions.size());
			b_hitTriggers.resize(hitTriggers.size());

			std::copy(hitCollisions.begin(), hitCollisions.end(), b_hitCollisions.begin());
			std::copy(hitTriggers.begin(), hitTriggers.end(), b_hitTriggers.begin());

			if (b_hitCollisions.size() != 0) {
				for (auto& com : gameObject->components) {
					if (com->type == eComponentType::Behaviour)
						MessageSystem::SendMessageToCom(noDel_ptr<Component>(com), L"CollisionExit");
				}
			}
			if (b_hitTriggers.size() != 0) {
				for (auto& com : gameObject->components) {
					if (com->type == eComponentType::Behaviour)
						MessageSystem::SendMessageToCom(noDel_ptr<Component>(com), L"TriggerExit");
				}
			}

			hitCollisions.clear();
			hitTriggers.clear();
			messages.clear();
		}
	}
}