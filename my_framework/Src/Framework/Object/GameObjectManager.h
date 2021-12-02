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

public:
	GameObjectManager() {};
	~GameObjectManager();

	void Execute();
	void Render();

	//オブジェクトの削除予約
	void ReserveDestroyObject(int objID);

	//オブジェクトクリエイター
	noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Transform> parent = nullptr);
	noDel_ptr<GameObject> CreateObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite, noDel_ptr<Transform> parent = nullptr);
	noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Mesh> mesh, noDel_ptr<Transform> parent = nullptr);

private:
	//配列から特定のIDのコンポーネントを抜く
	void PullOutComponent(noDel_ptr<GameObject> obj);
	void PullOutTransform(noDel_ptr<Component> com);
	void PullOutCollider2D(noDel_ptr<Component> com);
	void PullOutPhysics2D(noDel_ptr<Component> com);
	void PullOutSpriteRenderer(noDel_ptr<Component> com);
	void PullOutMeshRenderer(noDel_ptr<Component> com);
	void PullOutBehaviour(noDel_ptr<Component> com);
	void PullOutAnimator(noDel_ptr<Component> com);

	//コンポーネント処理の有効無効を確認する
	bool CheckComponentEnable(noDel_ptr<Component> com);

	//描画順の変更(クイックソート)
	void RenderOrderSort(int start, int end);
	
	//オブジェクトに追加されたコンポーネントをこのクラスの配列に格納
	void RegistComponent(noDel_ptr<GameObject> obj);

	//コンポーネントをダウンキャストした結果を返す
	template <class T>
	noDel_ptr<T> DownCastComponent(noDel_ptr<Component> com) {
		noDel_ptr<T> component = dynamic_noDel_cast<T>(com);
		if (component != nullptr) {
			return component;
		}
		return nullptr;
	}

	//コンポーネントを新たに配列に加える
	bool AddTransform(noDel_ptr<Component> com);
	bool AddCollider2D(noDel_ptr<Component> com);
	bool AddSpriteRenderer(noDel_ptr<Component> com);
	bool AddMeshRenderer(noDel_ptr<Component> com);
	bool AddPhysics2D(noDel_ptr<Component> com);
	bool AddBehaviour(noDel_ptr<Component> com);
	bool AddAnimator(noDel_ptr<Component> com);

public:
	//getter,setter
	void SetSceneType(int val) { sceneType = val; }
	void SetAddComponentTrigger() { isAddComponent = true; }
	void SetStartFuncEnable() { isStartFucnEnable = true; }

private:
	std::vector<GameObject*> objects;

	//コンポーネントの配列
	std::vector<noDel_ptr<Transform>> vTransform;
	std::vector<noDel_ptr<Collider2D>> vCollider2D;
	std::vector<noDel_ptr<Physics2D>> vPhysics2D;
	std::vector<noDel_ptr<SpriteRenderer>> vSpriteRenderer;
	std::vector<noDel_ptr<MeshRenderer>> vMeshRenderer;
	std::vector<noDel_ptr<Behaviour>> vBehaviour;
	std::vector<noDel_ptr<Animator>> vAnimator;

public:
	std::vector<int> vDestroyID; //削除するオブジェクトID
};
