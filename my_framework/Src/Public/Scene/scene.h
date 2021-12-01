/*-----------------------------------------------------------

	�V�[���N���X�i���N���X�j
		�e�V�[���̋��ʂ̃N���X�E���ۃN���X

-------------------------------------------------------------*/
#pragma once
#include "../InputConfig.h"



class Scene : public GameObjectManager, public MeshManager, public SoundManager {
public:

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

	//int�^�̐����́{�P�[�P���w��̕�(0~maxNum)�ōs��
	static int CountUpInRange(int target, int maxRange);
	static int CountDownInRange(int target, int maxRange);
};