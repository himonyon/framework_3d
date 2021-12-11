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
	bool isSortEnable = false; //スプライトのソートを実行するか

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
	void SetSortEnable() { isSortEnable = true; }

private:
	std::unordered_map<int, GameObject*> umObjects;

	//コンポーネントの配列
	std::unordered_map<int,noDel_ptr<Component>> umTransform;
	std::unordered_map<int,noDel_ptr<Component>> umCollider2D;
	std::unordered_map<int,noDel_ptr<Component>> umPhysics2D;
	std::vector<noDel_ptr<Component>> v2DRenderer;
	std::unordered_map<int,noDel_ptr<Component>> umMeshRenderer;
	std::unordered_map<int,noDel_ptr<Component>> umBehaviour;
	std::unordered_map<int,noDel_ptr<Component>> umAnimator;

public:
	std::vector<int> vDestroyID; //削除するオブジェクトID
};
