#pragma once
/*-----------------------------------------------------------

	ゲームオブジェクトクラス
		ゲームオブジェクトの管理

-------------------------------------------------------------*/

class SceneManager;

class GameObject : public Object {
public:
	//座標、回転、スケール変換構造体
	noDel_ptr<Transform> transform = 0;

	//コンポーネントが新たに追加されたか
	bool isAddNewComponent = false;

	//登録されたコンポーネント
	std::vector<Component*> components;

private:
	bool objEnable; //実行管理
	//オブジェクトの所属シーン(eSceneTable)
	int belongSceneType = 0;

public:
	GameObject();
	~GameObject(void); 

	//オブジェクトの破棄
	void Destroy(noDel_ptr<GameObject> obj = nullptr);

	//オブジェクトの作成
	//空オブジェクト作成
	noDel_ptr<GameObject> CreateObject(float x, float y, float z,
		noDel_ptr<Transform> parent = nullptr, bool local = false);
	//スプライトオブジェクト作成
	noDel_ptr<GameObject> CreateObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite,
		noDel_ptr<Transform> parent = nullptr, bool local = false);
	//メッシュオブジェクト作成
	noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Mesh> mesh,
		noDel_ptr<Transform> parent = nullptr, bool local = false);


	//コンポーネントの追加
	template<class T>
	void AddComponent() {
		T* instance = new T();
		instance->gameObject = noDel_ptr<GameObject>(this);
		if(transform != nullptr) instance->transform = transform;
		components.emplace_back(instance);
		isAddNewComponent = true;
		SceneManager::GetScene(belongSceneType)->SetAddComponentTrigger();
	}

	//コンポーネントの取得
	template<class T>
	noDel_ptr<T> GetComponent() {
		T* component;
		for (Component* com : components) {
			component = dynamic_cast<T*>(com);
			if (component != nullptr) {
				return noDel_ptr<T>(component);
			}
		}

		return nullptr;
	}

	//getter, setter
	void SetObjEnable(bool flag); //実行状態の設定
	bool IsObjEnable(); //実行状態になっているかどうか
	void SetSceneType(int value) { belongSceneType = value; }
	int GetSceneType() { return belongSceneType; }
};
