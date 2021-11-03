#pragma once

#define MAX_ANIMATION2D (50)

class SpriteAnimation {
private:
	bool animEnable = false;
	bool isLoop = false;
	float flameCount = 0; //�A�j���[�V�����t���[����


	Sprite* showSprite = NULL; //�\������X�v���C�g
	int currentKeyFlameIndex = -1; //���ݓ��B���Ă���L�[�t���[���z��ԍ�

public:
	//�A�j���[�V�����̃L�[�t���[��
	class KeyFlame {
	public:
		Sprite* pSprite;
		float flame;
		float x, y;
		float r, b, g, a;
		float scaleX, scaleY;
		float rot;
	};

private:
	std::vector<KeyFlame*> keyFlame;

public:
	SpriteAnimation(Sprite* pSprite, bool loop = false);

	bool isAnimEnable();
	void SetAnimEnable(bool flag);

	void AnimOn(); //�A�j���[�V�������s
	void AnimOff(); //�A�j���[�V�����I��


	//arrayIndex�͔z��̔ԍ����w��
	void SetKeyFlameSprite(int index, Sprite* sprite);
	void SetKeyFlameSprite(int index, Sprite* sprite, float flame);

	void SetKeyFlamePos(int index, float x, float y);
	void SetKeyFlamePos(int index, float x, float y, float flame);

	void SetKeyFlameCol(int index, float r, float g, float b, float a);
	void SetKeyFlameCol(int index, float r, float g, float b, float a, float flame);

	void SetKeyFlameScale(int index, float x, float y);
	void SetKeyFlameScale(int index, float x, float y, float flame);

	void SetKeyFlameRot(int index, float rot);
	void SetKeyFlameRot(int index, float rot, float flame);

private:
	void AnimLoop(); //�A�j���[�V������������x�Đ�
	void CreateKeyFlame(int keyNum);
};