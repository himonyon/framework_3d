#include "../../../../framework.h"
#include "../../../../environment.h"

Behaviour::Behaviour() : Component(eComponentType::Behaviour) {
}

void Behaviour::Execute() {
	for (auto& m : messages) {
		//�X�^�[�g
		if (startCalled == false) {
			if (m == L"Start") {
				Start();
				startCalled = true;
				break;
			}
		}

		//�A�b�v�f�[�g
		if (m == L"Update") {
			Update();
			break;
		}

		//�R���C�_�[�̏���(���b�Z�[�W�ɂ���R���C�_�[�̎�ސ��s��)

		//�R���C�_�[�������Ȃ�I��
		if (c2d == nullptr) c2d = gameObject->GetComponent<Collider2D>();
		if (c2d == nullptr) break;
		if (c2d->IsEnable() == false) break;

		//������͂��߁A�������Ă���Ԃ̏���
		if (m == L"Collision") {
			for (auto& hit : c2d->hitCollisions) {
				hit->AddHitCollisions(noDel_ptr<Collider2D>(c2d)); //����ɂ��q�b�g�����������A�j�����ꂽ�Ƃ����m�\�ɂ���
				MessageSystem::SendMessageToCom(hit, L"Clear");
				bool _trigger = false;
				for (auto& b_hit : c2d->b_hitCollisions) {
					if (hit == b_hit) _trigger = true;
				}
				if (_trigger) OnCollision2D(hit);
				else OnCollisionEnter2D(hit);
			}
			MessageSystem::SendMessageToCom(c2d, L"Clear");
		}
		if (m == L"Trigger") {
			for (auto& hit : c2d->hitTriggers) {
				hit->AddHitTriggers(noDel_ptr<Collider2D>(c2d));
				MessageSystem::SendMessageToCom(hit, L"Clear");
				bool _trigger = false;
				for (auto& b_hit : c2d->b_hitTriggers) {
					if (hit == b_hit) _trigger = true;
				}
				if (_trigger) OnTrigger2D(hit);
				else OnTriggerEnter2D(hit);
			}
			MessageSystem::SendMessageToCom(c2d, L"Clear");
		}

		//������𔲂������̏���
		if (m == L"CollisionExit") {
			for (auto& b_hit : c2d->b_hitCollisions) {
				bool _trigger = false;
				for (auto& hit : c2d->hitCollisions) {
					if (hit == b_hit) _trigger = true;
				}
				//�Y��������̂��Ȃ��ꍇ�A����Ă���̂ŏ���
				if (!_trigger) OnCollisionExit2D(b_hit);
				MessageSystem::SendMessageToCom(c2d, L"Clear");
			}
		}
		if (m == L"TriggerExit") {
			for (auto& b_hit : c2d->b_hitTriggers) {
				bool _trigger = false;
				for (auto& hit : c2d->hitTriggers) {
					if (hit == b_hit) _trigger = true;
				}
				//�Y��������̂��Ȃ��ꍇ�A����Ă���̂ŏ���
				if (!_trigger) {
					OnTriggerExit2D(b_hit);
				}
				MessageSystem::SendMessageToCom(c2d, L"Clear");
			}
		}
	}

	messages.clear();
}
