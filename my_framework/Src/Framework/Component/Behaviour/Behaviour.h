#pragma once
/*-----------------------------------------------------------

	Behaviourクラス
		オブジェクトの振る舞いを扱うクラス

-------------------------------------------------------------*/

enum class eBehaviourState {
	Awake,
	Update,
};

class Behaviour : public Component {
private:
	bool startCalled = false;

	noDel_ptr<Collider2D> c2d = 0;

public:
	Behaviour();
	virtual ~Behaviour() {};

	//コンポーネント処理
	void Execute() override;
	void Execute(int state) override;

private:
	//オブジェクトが作成されたときに呼び出される関数
	virtual void Awake() {};

	//最初のフレームで呼び出される関数
	virtual void Start() {};

	//毎フレーム呼び出される処理関数
	virtual void Update() {};

	//オブジェクトの衝突処理
	virtual void OnCollision2D(noDel_ptr<Collider2D> hitCollider) {};
	virtual void OnCollisionEnter2D(noDel_ptr<Collider2D> hitCollider) {};
	virtual void OnCollisionExit2D(noDel_ptr<Collider2D> exitCollider) {};
	virtual void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) {};
	virtual void OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) {};
	virtual void OnTriggerExit2D(noDel_ptr<Collider2D> exitCollider) {};

protected:
	noDel_ptr<Sprite> CreateSprite(Sprite* sprite);
};