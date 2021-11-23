#pragma once

class GameObjectManager {
public:
	int sceneType = 0;

	GameObjectManager() {};
	~GameObjectManager();

	//２Dオブジェクトクリエイター
	template<class T>
	T* CreateObject2D(GameObject2D* instance) {
		instance->belongSceneType = sceneType;
		instance->Start();
		objects2d.emplace_back(instance);
		return dynamic_cast<T*>(objects2d.back());
	}

	//３Dオブジェクトクリエイター
	template<class T>
	T* CreateObject3D(GameObject3D* instance) {
		instance->belongSceneType = sceneType;
		instance->Start();
		objects3d.emplace_back(instance);
		return dynamic_cast<T*>(objects3d.back());
	}

	//処理
	void Execute();
	//描画
	void Render();

private:
	//描画順の変更(クイックソート)
	void RenderOrderSort(int start, int end);

private:
	std::vector<GameObject2D*> objects2d;
	std::vector<GameObject3D*> objects3d;
};
