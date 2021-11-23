/*-----------------------------------------------------------

	�X�v���C�g�A�j���[�V�����N���X
		�L�[���t���[�����Ƌ��ɐݒ肵�Ċe�L�[�̎��ԂɂȂ��
		���̏������s���A�j���[�V�����@�\

-------------------------------------------------------------*/
#pragma once

#define MAX_ANIMATION2D (50)

class SpriteAnimation {
private:
	bool animEnable = false;
	bool isLoop = false;
	float flameCount = 0; //�A�j���[�V�����t���[����

	noDel_ptr<GameObject2D> pAnimObj = NULL; //�\������X�v���C�g
	int currentKeyFrameIndex = -1; //���ݓ��B���Ă���L�[�t���[���z��ԍ�

public:
	//�A�j���[�V�����̃L�[�t���[��
	class KeyFrame {
	public:
		noDel_ptr<Sprite> pSprite = 0;
		float flame = 0;
		float x = 0, y = 0;
		stColor4 color;;
		float scaleX = 0, scaleY = 0;
		float rot = 0;
	};

private:
	std::vector<KeyFrame*> keyFrame;

public:
	SpriteAnimation(noDel_ptr<GameObject2D> obj, bool loop = false);

	bool isAnimEnable();
	void SetAnimEnable(bool flag);

	void AnimOn(); //�A�j���[�V�������s
	void AnimOff(); //�A�j���[�V�����I��


	//arrayIndex�͔z��̔ԍ����w��
	void SetKeyFrameSprite(int keyIndex, noDel_ptr<Sprite> sprite);
	void SetKeyFrameSprite(int keyIndex, noDel_ptr<Sprite> sprite, float flame);

	void SetKeyFramePos(int keyIndex, float x, float y);
	void SetKeyFramePos(int keyIndex, float x, float y, float flame);

	void SetKeyFrameCol(int keyIndex, float r, float g, float b, float a);
	void SetKeyFrameCol(int keyIndex, float r, float g, float b, float a, float flame);

	void SetKeyFrameScale(int keyIndex, float x, float y);
	void SetKeyFrameScale(int keyIndex, float x, float y, float flame);

	void SetKeyFrameRot(int keyIndex, float rot);
	void SetKeyFrameRot(int keyIndex, float rot, float flame);

private:
	void AnimLoop(); //�A�j���[�V������������x�Đ�
	void CreateKeyFrame(int keyNum);
};