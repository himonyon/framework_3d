#include "../../framework.h"
#include "../../environment.h"


//�R���X�g���N�^
Scene::Scene()
{
}

//�f�X�g���N�^
Scene::~Scene()
{
}

//����
void Scene::Execute()
{
	//�摜�̏���
	GameObjectManager::Execute();
	AnimationManager::Execute();
}

void Scene::Render()
{
	GameObjectManager::Render();
}

//�X�v���C�g���m�̏Փ˂����o����
//�߂�l eCollideState : �Փ˂̏��
//����
//vnBlock2D *p1 : �Փ˂����o����1�ڂ̃I�u�W�F�N�g
//vnBlock2D *p2 : �Փ˂����o����2�ڂ̃I�u�W�F�N�g
//bool resolve : �Փ˂̉������s��(p1�𓮂����āAp2�͌Œ肷��)
Scene::eCollideState Scene::IsCollide2D(noDel_ptr<GameObject2D> p1, noDel_ptr<GameObject2D> p2, bool resolve)
{
	//�����̃G���[�`�F�b�N
	if (p1 == NULL || p2 == NULL)
	{
		return eCollideState::None;
	}

	//�߂�l�p�̕ϐ�
	eCollideState hit = eCollideState::None;

	//�Փ˂̌��o
	float rx = 0.0f;	//�d�Ȃ��Ă��钷���`(Rectangle)�̕�
	float ry = 0.0f;	//�d�Ȃ��Ă��钷���`(Rectangle)�̍���

	//��(X)����

	//�X�v���C�g�̒��S���m�̍��W�̍���(differencial)
	float dx = fabsf(p1->position.x - p2->position.x);
	//2�̃X�v���C�g�̕�(����)���X�P�[���̍��v
	float sx = p1->sizeX * 0.5f * p1->scale.x + p2->sizeX * 0.5f * p2->scale.x;

	if (dx < sx)	//����������菬������Ή�(X)�����œ������Ă���
	{
		//�c(Y)����

		//�X�v���C�g�̒��S���m�̍��W�̍���(differencial)
		float dy = fabsf(p1->position.y - p2->position.y);
		//2�̃X�v���C�g�̍���(����)���X�P�[���̍��v
		float sy = p1->sizeY * 0.5f * p1->scale.y + p2->sizeY * 0.5f * p2->scale.y;

		if (dy < sy)	//������������菬������Ώc(Y)�����œ������Ă���
		{
			//�������Ă��Ȃ��Ƃ�����񂩂瓖�������Ƃ������ɏ���������
			hit = eCollideState::FromLeft;

			//�d�Ȃ��Ă��钷���`�̃T�C�Y��ێ�
			rx = sx - dx;
			ry = sy - dy;

			Font::Print(640, 40, L"r : %.3f, %.3f", rx, ry);
		}
	}

	//�Փ˂�����ꍇ�A�߂荞�݂���������
	//p1�𓮂����āAp2�͓������Ȃ�
	if (hit != eCollideState::None && resolve == true)
	{
		if (rx < ry)
		{	//�d�Ȃ����̈�(�����`)���c�������E����Փ�
			if (p1->position.x < p2->position.x)
			{	//p1��������Փ�
				Font::Print(640, 60, L"from Left");
				p1->position.x -= rx;
				hit = eCollideState::FromLeft;
			}
			else
			{	//p1���E����Փ�
				Font::Print(640, 60, L"from Right");
				p1->position.x += rx;
				hit = eCollideState::FromRight;
			}
		}
		else
		{	//�d�Ȃ����̈�(�����`)���������㉺����Փ�
			if (p1->position.y < p2->position.y)
			{	//p1���ォ��Փ�
				Font::Print(640, 60, L"from Top");
				p1->position.y -= ry;
				hit = eCollideState::FromTop;
			}
			else
			{	//p1��������Փ�
				Font::Print(640, 60, L"from Bottom");
				p1->position.y += ry;
				hit = eCollideState::FromBottom;
			}
		}
	}

	return hit;
}

