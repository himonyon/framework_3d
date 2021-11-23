#pragma once

class GameObjectManager {
public:
	int sceneType = 0;

	GameObjectManager() {};
	~GameObjectManager();

	//�QD�I�u�W�F�N�g�N���G�C�^�[
	template<class T>
	T* CreateObject2D(GameObject2D* instance) {
		instance->belongSceneType = sceneType;
		instance->Start();
		objects2d.emplace_back(instance);
		return dynamic_cast<T*>(objects2d.back());
	}

	//�RD�I�u�W�F�N�g�N���G�C�^�[
	template<class T>
	T* CreateObject3D(GameObject3D* instance) {
		instance->belongSceneType = sceneType;
		instance->Start();
		objects3d.emplace_back(instance);
		return dynamic_cast<T*>(objects3d.back());
	}

	//����
	void Execute();
	//�`��
	void Render();

private:
	//�`�揇�̕ύX(�N�C�b�N�\�[�g)
	void RenderOrderSort(int start, int end);

private:
	std::vector<GameObject2D*> objects2d;
	std::vector<GameObject3D*> objects3d;
};
