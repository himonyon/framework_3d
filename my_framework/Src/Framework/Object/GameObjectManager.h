#pragma once
/*-----------------------------------------------------------

	GameObjectManagerクラス(基底クラス)
		ゲームオブジェクトのオブジェクトプール
		ゲームオブジェクトの管理

-------------------------------------------------------------*/

class GameObjectManager {
private:
	int sceneType = 0;

	bool isAddComponent = false; //新たにコンポーネントを追加したか
	bool isStartFucnEnable = false; //スタート関数を実行するか
	bool isSpriteSortEnable = false; //スプライトのソートを実行するか

public:
	GameObjectManager() {};
	~GameObjectManager();

	void Execute();
	void Render();

	//オブジェクトの削除予約
	void ReserveDestroyObject(int objID);

	//オブジェクトクリエイタ
	//空オブジェクト作成
	noDel_ptr<GameObject> CreateObject(float x, float y, float z,
		noDel_ptr<Transform> parent = nullptr, bool local = false);
	//スプライトオブジェクト作成
	noDel_ptr<GameObject> CreateObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite,
		noDel_ptr<Transform> parent = nullptr, bool local = false);
	//メッシュオブジェクト作成
	noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Mesh> mesh, 
		noDel_ptr<Transform> parent = nullptr, bool local = false);

private:
	//配列から特定のIDのコンポーネントを抜く
	void PullOutComponent(noDel_ptr<GameObject> obj);

	//コンポーネント処理の有効無効を確認する
	bool CheckComponentEnable(noDel_ptr<Component> com);

	//描画順の変更(クイックソート)
	void RenderOrderSort(int start, int end);
	
	//オブジェクトに追加されたコンポーネントをこのクラスの配列に格納
	void RegistComponent(noDel_ptr<GameObject> obj);


public:
	//getter,setter
	void SetSceneType(int val) { sceneType = val; }
	void SetAddComponentTrigger() { isAddComponent = true; }
	void SetStartFuncEnable() { isStartFucnEnable = true; }
	void SetSpriteSortEnable() { isSpriteSortEnable = true; }

private:
	std::vector<GameObject*> objects;

	//コンポーネントの配列
	std::vector<noDel_ptr<Component>> vTransform;
	std::vector<noDel_ptr<Component>> vCollider2D;
	std::vector<noDel_ptr<Component>> vPhysics2D;
	std::vector<noDel_ptr<Component>> vSpriteRenderer;
	std::vector<noDel_ptr<Component>> vMeshRenderer;
	std::vector<noDel_ptr<Component>> vBehaviour;
	std::vector<noDel_ptr<Component>> vAnimator;

public:
	std::vector<int> vDestroyID; //削除するオブジェクトID
};
