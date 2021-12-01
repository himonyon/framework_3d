#pragma once
/*-----------------------------------------------------------

	Transformクラス
		座標、回転、スケール変換

-------------------------------------------------------------*/

class Transform : public Component {
public:
	stVector3 position; //座標
	stVector3 rotation; //回転
	stVector3 scale; //スケール

	//親から見た相対座標
	stVector3 localPosition;
	stVector3 localRotation;
	stVector3 localScale;

	bool isChanged = false; //Transformに変更があったか
private:
	noDel_ptr<Transform> pParent; //親のオブジェクト
	std::vector<noDel_ptr<Transform>> pChildren; //子オブジェクト

	stVector3 b_position;
	stVector3 b_rotation;
	stVector3 b_scale;

	stVector3 b_localPosition;
	stVector3 b_localRotation;
	stVector3 b_localScale;
public:
	Transform(float x, float y, float z, noDel_ptr<Transform> parent);
	~Transform();

	//コンポーネント処理
	void Execute() override;

	//親のオブジェクトが有効状態かどうか
	bool IsParentObjEnable();

	noDel_ptr<Transform> GetParent() { return pParent; }
	void SetParent(noDel_ptr<Transform> obj);
	void SetPosition(float x, float y);
	void SetScale(float x, float y);
	void SetRotation(float x, float y, float z);

private:
	//ローカルとグローバルの変更を互いに反映させる
	void ConvertLocalAndGlobal();

	//オブジェクトの座標、回転、スケールの更新
	void UpdateState();

	//親オブジェクトに対して子オブジェクトの座標、回転、スケールを相対的に設定
	void SetRelativeState();

	//値の有効桁数を少数３桁にする
	void RoundingTransform();
};
