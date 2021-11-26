/*-----------------------------------------------------------

	�V�[���N���X�i���N���X�j
		�e�V�[���̋��ʂ̃N���X�E���ۃN���X

-------------------------------------------------------------*/
#pragma once
#include "../InputConfig.h"

class Scene : public GameObjectManager, public SpriteManager, public MeshManager, public SoundManager, public AnimationManager {
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

	//�����蔻��
	enum class eCollideState	//�Փ˂̏�Ԃ�\���񋓎q(enum)
	{
		None,		//�Փ˂��Ă��Ȃ�
		FromLeft,	//��������Փ�
		FromRight,	//�E������Փ�
		FromTop,	//�㑤����Փ�
		FromBottom,	//��������Փ�
	};
	//bool resolve : �Փ˂̉������s��(p1�𓮂����āAp2�͌Œ肷��)
	static eCollideState IsCollide2D(noDel_ptr<GameObject2D> p1, noDel_ptr<GameObject2D> p2, bool resolve = true);

	//int�^�̐����́{�P�[�P���w��̕�(0~maxNum)�ōs��
	static int CountUpInRange(int target, int maxRange);
	static int CountDownInRange(int target, int maxRange);
};