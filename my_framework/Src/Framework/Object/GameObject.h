#pragma once
/*-----------------------------------------------------------

	ゲームオブジェクトクラス
		ゲームオブジェクトの管理

-------------------------------------------------------------*/

class SceneManager;

class GameObject : public Object {
public:
	//座標、回転、スケール変換構造体
	noDel_ptr<Transform> transform;

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

	//getter, setter
	void SetObjEnable(bool flag); //実行状態の設定
	bool IsObjEnable(); //実行状態になっているかどうか

	//コンポーネントの追加
	template<class T>
	void AddComponent(Component* instance) {
		instance->SetObjectID(GetObjectID());
		instance->gameObject = noDel_ptr<GameObject>(this);
		if (instance->gameObject->transform != nullptr) instance->transform = instance->gameObject->transform;
		T* newInstance = dynamic_cast<T*>(instance);
		components.emplace_back(newInstance);
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

	void SetSceneType(int value) { belongSceneType = value; }
	int GetSceneType() { return belongSceneType; }
};
