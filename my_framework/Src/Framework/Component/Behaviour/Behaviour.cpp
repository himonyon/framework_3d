#include "../../../../framework.h"
#include "../../../../environment.h"

Behaviour::Behaviour() : Component(eComponentType::Behaviour) {
}

void Behaviour::Execute() {
	for (auto& m : messages) {
		//スタート
		if (startCalled == false) {
			if (m == L"Start") {
				Start();
				startCalled = true;
				break;
			}
		}

		//アップデート
		if (m == L"Update") {
			Update();
			break;
		}

		//コライダーの処理(メッセージにあるコライダーの種類数行う breakしない)
		if (m == L"OnCollision2D") {
			if (c2d == nullptr) c2d = gameObject->GetComponent<Collider2D>(); //コライダー取得
			if (c2d == nullptr) continue;
			if (c2d->IsEnable() == false) continue;
			for (auto& hit : c2d->hitCollisions) {
				if (hit->IsEnable() == false) continue;
				OnCollision2D(hit);
			}
			MessageSystem::SendMessageToCom(c2d, L"Clear");
		}

		if (m == L"OnTrigger2D") {
			if (c2d == nullptr) c2d = gameObject->GetComponent<Collider2D>(); //コライダー取得
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
