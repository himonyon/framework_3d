#pragma once
/*-----------------------------------------------------------

	Behaviourクラス
		オブジェクトの振る舞いを扱うクラス

-------------------------------------------------------------*/

class Behaviour : public Component {
private:
	bool startCalled = false;

	noDel_ptr<Collider2D> c2d = 0;

public:
	Behaviour();
	virtual ~Behaviour() {};

	//コンポーネント処理
	void Execute() override;

private:
	//オブジェクトが作られたときに呼び出される関数
	virtual void Start() = 0;

	//毎フレーム呼び出される処理関数
	virtual void Update() = 0;

	//オブジェクトの衝突処理
	virtual void OnCollision2D(noDel_ptr<Collider2D> hitCollider) {};
	virtual void OnCollisionEnter2D(noDel_ptr<Collider2D> hitCollider) {};
	virtual void OnCollisionExit2D(noDel_ptr<Collider2D> exitCollider) {};
	virtual void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) {};
	virtual void OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) {};
	virtual void OnTriggerExit2D(noDel_ptr<Collider2D> exitCollider) {};
};