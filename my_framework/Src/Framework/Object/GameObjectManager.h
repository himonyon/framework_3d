#pragma once
/*-----------------------------------------------------------

	GameObjectManagerクラス(基底クラス)
		ゲームオブジェクトのオブジェクトプール
		ゲームオブジェクトの管理

-------------------------------------------------------------*/

namespace MyFrameWork {
	class GameObjectManager {
	private:
		int sceneType = 0;

		bool isStartFucnEnable = false; //スタート関数を実行するか

		bool isSortEnable = false; //スプライトのソートを実行するか

	public:
		GameObjectManager() {};
		~GameObjectManager();

		//処理
		void Execute();
		//描画
		void Render();

		//オブジェクトの削除予約
		void ReserveDestroyObject(std::string name);

		//オブジェクトの有効予約
		void ReserveEnableObject(std::string name);

		//オブジェクトクリエイタ
		//空オブジェクト作成
		noDel_ptr<GameObject> CreateObject(float x, float y, float z,
			noDel_ptr<Transform> parent = nullptr, std::string name = "");
		//スプライトオブジェクト作成
		noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Sprite> sprite,
			noDel_ptr<Transform> parent = nullptr, std::string name = "");
		//メッシュオブジェクト作成
		noDel_ptr<GameObject> CreateObject(float x, float y, float z, noDel_ptr<Mesh> mesh,
			noDel_ptr<Transform> parent = nullptr, std::string name = "");
		//イメージ(UI)オブジェクト作成
		noDel_ptr<GameObject> CreateImageObject(float x, float y, float width, float height, noDel_ptr<Sprite> sprite,
			noDel_ptr<Transform> parent = nullptr, std::string name = "");

	private:
		//配列から特定のIDのコンポーネントを抜く
		void PullOutComponent(noDel_ptr<GameObject> obj);
		//Vector型の配列からコンポーネントを抜く
		void PullOutComponentToVector(std::vector<noDel_ptr<Component>>& renderer, noDel_ptr<Component> com);

		//コンポーネント処理の有効無効を確認する
		bool CheckComponentEnable(noDel_ptr<Component> com);

		//描画順の変更(クイックソート)
		void RenderOrderSort(std::vector<noDel_ptr<Component>>& renderer, int start, int end);

		//Vector型にコンポーネントを登録する
		void RegistComponentToVector(std::vector<noDel_ptr<Component>>& renderer, noDel_ptr<Component> com);

	public:
		//オブジェクトに追加されたコンポーネントをこのクラスの配列に格納
		void RegistComponent(noDel_ptr<Component> com);

		//ゲームオブジェクトを取得する
		noDel_ptr<GameObject> GetGameObject(std::string name);

		//getter,setter
		void SetSceneType(int val) { sceneType = val; }
		int GetSceneType() const { return sceneType; }
		void SetStartFuncEnable() { isStartFucnEnable = true; }
		void SetSortEnable() { isSortEnable = true; }

	private:
		//ゲームオブジェクト配列
		std::unordered_map<std::string, GameObject*> umObjects;

		//コンポーネントの配列
		std::unordered_map<int, noDel_ptr<Component>> umTransform;
		std::unordered_map<int, noDel_ptr<Component>> umCollider2D;
		std::unordered_map<int, noDel_ptr<Component>> umPhysics2D;
		std::vector<noDel_ptr<Component>> vImageRenderer;
		std::vector<noDel_ptr<Component>> vSpriteRenderer;
		std::vector<noDel_ptr<Component>> vMeshRenderer;
		std::unordered_map<int, noDel_ptr<Component>> umBehaviour;
		std::unordered_map<int, noDel_ptr<Component>> umAnimator;

		//予約して一括操作するためのコンテナ
		std::vector<std::string> vDestroyName; //削除するオブジェクト名
		std::vector<std::string> vEnableName; //有効・無効するオブジェクト名(Behaviourの即時実行を防ぐ)
	};
}