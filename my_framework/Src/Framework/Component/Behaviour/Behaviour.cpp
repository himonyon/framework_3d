#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

Behaviour::Behaviour() : Component(eComponentType::Behaviour) {
}

void Behaviour::Execute() {
	for (auto& m : messages) {
		//コライダーの処理(メッセージにあるコライダーの種類数行う)
		//コライダーが無効なら終了
		if (c2d == nullptr) c2d = gameObject->GetComponent<Collider2D>();
		if (c2d == nullptr) break;
		if (c2d->IsEnable() == false) break;

		//当たりはじめ、当たっている間の処理
		if (m == L"Collision") {
			for (auto& hit : c2d->hitCollisions) {
				hit->AddHitCollisions(noDel_ptr<Collider2D>(c2d)); //相手にもヒット情報を持たせ、破棄されたとき検知可能にする
				MessageSystem::SendMessageToCom(static_noDel_cast<Component>(hit), L"Clear");
				bool _trigger = false;
				//前フレームの当たり判定を見て処理を分ける
				for (auto& b_hit : c2d->b_hitCollisions) {
					if (hit == b_hit) _trigger = true;
				}
				if (_trigger) OnCollision2D(hit);
				else OnCollisionEnter2D(hit);
			}
			MessageSystem::SendMessageToCom(static_noDel_cast<Component>(c2d), L"Clear");
		}
		if (m == L"Trigger") {
			for (auto& hit : c2d->hitTriggers) {
				hit->AddHitTriggers(noDel_ptr<Collider2D>(c2d));
				MessageSystem::SendMessageToCom(static_noDel_cast<Component>(hit), L"Clear");
				bool _trigger = false;
				for (auto& b_hit : c2d->b_hitTriggers) {
					if (hit == b_hit) _trigger = true;
				}
				if (_trigger) OnTrigger2D(hit);
				else OnTriggerEnter2D(hit);
			}
			MessageSystem::SendMessageToCom(static_noDel_cast<Component>(c2d), L"Clear");
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
				MessageSystem::SendMessageToCom(static_noDel_cast<Component>(c2d), L"Clear");
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
				MessageSystem::SendMessageToCom(static_noDel_cast<Component>(c2d), L"Clear");
			}
		}
	}

	messages.clear();
}

void Behaviour::Execute(int state) {
	if (state == (int)eBehaviourState::Update) {
		//スタート
		if (startCalled == false) {
			Start();
			startCalled = true;
		}
		else {
			Update();
		}
	}
	else if(state == (int)eBehaviourState::Awake) {
		Awake();
	}
}

noDel_ptr<Sprite> Behaviour::CreateSprite(Sprite* sprite) {
	return SceneManager::GetScene(gameObject->GetSceneType())->CreateSprite(sprite);
}