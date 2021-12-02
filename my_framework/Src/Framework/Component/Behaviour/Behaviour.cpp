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

		//コライダーの処理(メッセージにあるコライダーの種類数行う)

		//コライダーが無効なら終了
		if (c2d == nullptr) c2d = gameObject->GetComponent<Collider2D>();
		if (c2d == nullptr) break;
		if (c2d->IsEnable() == false) break;

		//当たりはじめ、当たっている間の処理
		if (m == L"Collision") {
			for (auto& hit : c2d->hitCollisions) {
				hit->AddHitCollisions(noDel_ptr<Collider2D>(c2d)); //相手にもヒット情報を持たせ、破棄されたとき検知可能にする
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

		//当たりを抜けた時の処理
		if (m == L"CollisionExit") {
			for (auto& b_hit : c2d->b_hitCollisions) {
				bool _trigger = false;
				for (auto& hit : c2d->hitCollisions) {
					if (hit == b_hit) _trigger = true;
				}
				//該当するものがない場合、離れているので処理
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
				//該当するものがない場合、離れているので処理
				if (!_trigger) {
					OnTriggerExit2D(b_hit);
				}
				MessageSystem::SendMessageToCom(c2d, L"Clear");
			}
		}
	}

	messages.clear();
}
