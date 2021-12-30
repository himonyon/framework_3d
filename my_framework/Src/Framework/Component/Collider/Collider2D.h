#pragma once
/*-----------------------------------------------------------

	コライダー2Dクラス
		2Dの四角形の当たり判定

-------------------------------------------------------------*/

class Collider2D : public Component {
public:
	bool isCollision = true;
	float sizeX = 0;
	float sizeY = 0;

	//衝突しているコライダー
	std::vector<noDel_ptr<Collider2D>> hitCollisions;
	std::vector<noDel_ptr<Collider2D>> hitTriggers;

	//前フレームで衝突したコライダー
	std::vector<noDel_ptr<Collider2D>> b_hitCollisions; 
	std::vector<noDel_ptr<Collider2D>> b_hitTriggers;

public:
	Collider2D();
	~Collider2D();

	//コンポーネントの初期化
	void SetUpCollider2D(bool collision);
	void SetUpCollider2D(float sizeX, float sizeY, bool collision);

	//コンポーネント処理
	void Execute();
	void Execute(noDel_ptr<Collider2D> hitCollider) override;
	void AddHitCollisions(noDel_ptr<Collider2D> hitColider);
	void AddHitTriggers(noDel_ptr<Collider2D> hitColider);

	//当たり状態のクリア
	void ClearHitState();

private:
	void IsCollide(noDel_ptr<Collider2D> hitColider);
};