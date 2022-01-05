#pragma once
/*-----------------------------------------------------------

	コンポーネントクラス(基底クラス)
		オブジェクトと各種機能クラスを結びつけるクラス

-------------------------------------------------------------*/

class GameObject;
class Transform;
class Collider2D;

enum class eComponentType {
	None,
	Transform,
	Behaviour,
	WorldRenderer,
	ScreenRenderer,
	Collider,
	Physics,
	Animator,
	Camera,
};

class Component : public Object, public MessageSystem {
private:
	bool isRegisted = false;

	bool isEnable = true;

public:
	noDel_ptr<GameObject> gameObject = 0;

	noDel_ptr<Transform> transform = 0;

	eComponentType type = eComponentType::None;

public:
	Component(eComponentType type);
	virtual ~Component() {};

	//コンポーネントの各処理実行関数
	virtual void Execute() {};
	virtual void Execute(int state) {};
	virtual void Execute(noDel_ptr<Collider2D> col) {};

	bool IsEnable() { return isEnable; }

	bool IsRegisted() { return isRegisted; }
	void SetRegistState(bool flag);

	void SetEnable(bool flag);
};
