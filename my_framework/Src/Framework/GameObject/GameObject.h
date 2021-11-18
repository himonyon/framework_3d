#pragma once



class GameObject {
public:

	//座標
	stVector3 position;
	//スケール
	stVector3 scale;
	//回転(ラジアン)
	stVector3 rot;

protected:
	noDel_ptr<GameObject> pParent; //親のオブジェクト
	std::vector<noDel_ptr<GameObject>> pChildren; //子オブジェクト

	bool executeEnable; //実行管理
	bool renderEnable; //描画管理

public:
	//前フレームでの座標
	stVector3 before_position;
	//前フレームでのスケール
	stVector3 before_scale;
	//前フレームでの回転(ラジアン)
	stVector3 before_rot;

public:
	GameObject(float x, float y, float z, bool isRender = true, noDel_ptr<GameObject> parent = nullptr);
	~GameObject(void);

	virtual void Render(void);
	virtual void Execute(void);

	//getter, setter
	noDel_ptr<GameObject> GetParent() { return pParent; }
	void SetParent(noDel_ptr<GameObject> obj);
	void SetPosition(float x, float y);
	void SetScale(float x, float y);
	void SetRotation(float x, float y, float z);
	void SetRenderEnable(bool flag); //描画状態の設定
	bool isRenderEnable(); //描画状態になっているか
	void SetExecuteEnable(bool flag); //実行状態の設定
	bool isExecuteEnable(); //実行状態になっているかどうか
};
