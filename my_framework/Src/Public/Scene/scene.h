#pragma once

#define MaxSprites (256) //�P�V�[���ɊǗ��ł���摜��
#define MaxAnimation (64) //�P�V�[���ɊǗ��ł���A�j���[�V����

class Scene {
private:
	Sprite* pObject2D_array[MaxSprites];
	std::vector<SpriteAnimation*> pSpriteAnimation_array;

public:
	//�R���X�g���N�^
	Scene();

	//�f�X�g���N�^
	virtual ~Scene();

	//������
	virtual bool Initialize() = 0;

	virtual void Terminate() = 0;

	//����
	virtual void Execute();

	//�`��
	virtual void Render();


	bool RegisterObject(Sprite* p);
	void DeleteObject(Sprite* p);

	bool RegisterAnimation(SpriteAnimation* anim);


	//�����蔻��
	static bool isCollider(Sprite* collider, float x, float y); //�摜�ƍ��W
	static bool isCollider(Sprite* collider, int x, int y); //�摜�ƍ��W
private:
	void RenderOrderSort(int start, int end);
};