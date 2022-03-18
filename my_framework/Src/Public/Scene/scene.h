/*-----------------------------------------------------------

	�V�[���N���X�i���N���X�j
		�e�V�[���̋��ʂ̃N���X�E���ۃN���X

-------------------------------------------------------------*/
#pragma once
#include "../InputConfig.h"

enum class eRenderOrder {
	Background = -30,
	BackObject = -20,
	BackUI = -10,
	Object = 0,
	UI = 10,
	FrontObject = 20,
	FrontUI = 30,
};

class Scene : public GameObjectManager,public SpriteManager, public MeshManager {
public:
	bool isInitialized = false;

public:
	//�R���X�g���N�^
	Scene();

	//�f�X�g���N�^
	virtual ~Scene();

	//������
	virtual void Initialize() = 0;

	virtual void Terminate() = 0;

	//����
	virtual void Execute();

	//�`��
	virtual void Render();
};