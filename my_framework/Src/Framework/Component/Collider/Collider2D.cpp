#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

Collider2D::Collider2D() : Component(eComponentType::Collider) {
	sizeX = 0;
	sizeY = 0;
	isCollision = true;
}

Collider2D::~Collider2D() {
	//ヒット情報にいる相手のコライダーのヒット情報から自身を抜く
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
	noDel_ptr<SpriteState> sr = gameObject->GetComponent<SpriteState>();

	if (sr != nullptr) {
		sizeX = sr->size.x;
		sizeY = sr->size.y;
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
	//Objectの座標系が違えば処理しない
	if (gameObject->IsScreenObj() != hitCollider->gameObject->IsScreenObj()) return;

	//当たり判定チェック
	IsCollide(hitCollider);
}

//コライダー同士の衝突を検出する
void Collider2D::IsCollide(noDel_ptr<Collider2D> hitCollider)
{
	//引数のエラーチェック
	if (hitCollider == NULL)
	{
		return;
	}

	stVector3 hitPos = hitCollider->transform->position;
	stVector3 hitRot = hitCollider->transform->rotation;
	stVector3 hitScl = hitCollider->transform->scale;

	//戻り値用の変数
	bool hit = false;

	//衝突の検出
	float rx = 0.0f;	//重なっている長方形(Rectangle)の幅
	float ry = 0.0f;	//重なっている長方形(Rectangle)の高さ

	//横(X)方向

	//スプライトの中心同士の座標の差分(differencial)
	float dx = fabsf(transform->position.x - hitPos.x);
	//2つのスプライトの幅(半分)をスケールの合計
	float sx = sizeX * 0.5f * abs(transform->scale.x) + hitCollider->sizeX * 0.5f * abs(hitScl.x);

	if (dx < sx)	//差分が幅より小さければ横(X)方向で当たっている
	{
		//縦(Y)方向

		//スプライトの中心同士の座標の差分(differencial)
		float dy = fabsf(transform->position.y - hitPos.y);
		//2つのスプライトの高さ(半分)をスケールの合計
		float sy = sizeY * 0.5f * transform->scale.y + hitCollider->sizeY * 0.5f * hitScl.y;

		if (dy < sy)	//差分が高さより小さければ縦(Y)方向で当たっている
		{
			//当たっていないという情報から当たったという情報に書き換える
			hit = true;

			//重なっている長方形のサイズを保持
			rx = sx - dx;
			ry = sy - dy;
		}
	}


	if (hit == false) return;

	//衝突がある場合、めり込みを解消する
	//rigidを持っている方を動かす
	if (isCollision && hitCollider->isCollision)
	{
		if (rx < ry)
		{	//重なった領域(長方形)が縦長→左右から衝突
			if (transform->position.x < hitCollider->transform->position.x)
			{	//p1が左から衝突
				transform->position.x -= rx;
			}
			else
			{	//p1が右から衝突
				transform->position.x += rx;
			}
		}
		else
		{	//重なった領域(長方形)が横長→上下から衝突
			if (transform->position.y < hitCollider->transform->position.y)
			{	//p1が上から衝突
				transform->position.y -= ry;
			}
			else
			{	//p1が下から衝突
				transform->position.y += ry;
			}
		}
	}

	//ヒットオブジェクト配列に格納
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

	//オブジェクトにメッセージ送信
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

void Collider2D::ClearHitState() {
	//前フレームでの衝突をリセット&beforeCollsionの更新
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