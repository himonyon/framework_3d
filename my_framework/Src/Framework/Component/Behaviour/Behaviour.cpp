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

		//�R���C�_�[�̏���(���b�Z�[�W�ɂ���R���C�_�[�̎�ސ��s�� break���Ȃ�)
		if (m == L"OnCollision2D") {
			if (c2d == nullptr) c2d = gameObject->GetComponent<Collider2D>(); //�R���C�_�[�擾
			if (c2d == nullptr) continue;
			if (c2d->IsEnable() == false) continue;
			for (auto& hit : c2d->hitCollisions) {
				if (hit->IsEnable() == false) continue;
				OnCollision2D(hit);
			}
			MessageSystem::SendMessageToCom(c2d, L"Clear");
		}

		if (m == L"OnTrigger2D") {
			if (c2d == nullptr) c2d = gameObject->GetComponent<Collider2D>(); //�R���C�_�[�擾
			if (c2d == nullptr) continue;
			if (c2d->IsEnable() == false) continue;
			for (auto& hit : c2d->hitTriggers) {
				if (hit->IsEnable() == false) continue;
				OnTrigger2D(hit);
			}
			MessageSystem::SendMessageToCom(c2d, L"Clear");
		}
	}

	messages.clear();
}
