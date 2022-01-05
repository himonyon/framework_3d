/*-----------------------------------------------------------

	�X�v���C�g�A�j���[�V�����N���X
		�L�[���t���[�����Ƌ��ɐݒ肵�Ċe�L�[�̎��ԂɂȂ��
		���̏������s���A�j���[�V�����@�\

-------------------------------------------------------------*/
#pragma once

class SpriteAnimation {
private:
	bool isLoop = false;

	bool isEnd = false;

	float frameCount = 0; //�i�񂾃t���[����

	int curKeyIndex = -1; //���ݓ��B���Ă���L�[�t���[���z��ԍ�

public:
	noDel_ptr<Renderer2D> pAnimRenderer = NULL; //�\������X�v���C�g�����_���[
	//�A�j���[�V�����̃L�[�t���[��
	class KeyFrame {
	public:
		Sprite* pSprite = 0;
		float frame = 0;
		float x = 0, y = 0;
		stColor4 color = {};
		float scaleX = 0, scaleY = 0;
		float rot = 0;

	public:
		KeyFrame() {};
		~KeyFrame();
	};

private:
	std::vector<KeyFrame*> keyFrames;

public:
	SpriteAnimation(const WCHAR* animation_file, bool loop = false);
	~SpriteAnimation();

	void AnimOn(); //�A�j���[�V�������s
	void AnimOff(); //�A�j���[�V�����I��

	bool IsLoop() { return isLoop; }
	bool IsEnd() { return isEnd; }
};