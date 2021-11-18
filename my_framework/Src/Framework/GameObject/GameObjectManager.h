#pragma once

class GameObjectManager {
public:
	GameObjectManager() {};
	~GameObjectManager();

	//�QD�I�u�W�F�N�g�N���G�C�^�[
	noDel_ptr<GameObject2D> CreateObject2D(GameObject2D* instance);
	noDel_ptr<GameObject2D> CreateObject2D(float x, float y, float width, float height,
		noDel_ptr<Sprite> sprite, bool isRender = true, noDel_ptr<GameObject> parent = nullptr);

	//�RD�I�u�W�F�N�g�N���G�C�^�[
	noDel_ptr<GameObject3D> CreateObject3D(GameObject3D* instance);
	noDel_ptr<GameObject3D> CreateObject3D(float x, float y, float z,
		noDel_ptr<Mesh> mesh, bool isRender = true, noDel_ptr<GameObject> parent = nullptr);

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
