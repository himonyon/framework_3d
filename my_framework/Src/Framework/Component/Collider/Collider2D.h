#pragma once
/*-----------------------------------------------------------

	コライダー2Dクラス
		2Dの四角形の当たり判定

-------------------------------------------------------------*/

class Collider2D : public Component {
public:
	bool isCollision = true;
	float sizeX;
	float sizeY;

	//衝突しているコライダー
	std::vector<noDel_ptr<Collider2D>> hitCollisions;
	std::vector<noDel_ptr<Collider2D>> hitTriggers;

public:
	Collider2D(bool collision = true);
	Collider2D(float width, float height, bool collision = true);
	~Collider2D() {};

	//コンポーネント処理
	void Execute(noDel_ptr<Collider2D> hitCollider) override;

private:
	void IsCollide(noDel_ptr<Collider2D> hitColider);

	void AddHitCollisions(noDel_ptr<Collider2D> hitColider);
	void AddHitTriggers(noDel_ptr<Collider2D> hitColider);
};