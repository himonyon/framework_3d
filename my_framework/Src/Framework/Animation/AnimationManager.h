/*-----------------------------------------------------------

	�A�j���[�V�����Ǘ��N���X
		�V�[���ɍ��ꂽ�A�j���[�V�����̊Ǘ�
		�A�j���[�V�����N���X�C���X�^���X�̏��L�������z������L

-------------------------------------------------------------*/
#pragma once

class AnimationManager {
public:
	AnimationManager() {};
	~AnimationManager();

	noDel_ptr<SpriteAnimation> CreateAnimation(SpriteAnimation* instance);
	noDel_ptr<SpriteAnimation> CreateAnimation(noDel_ptr<GameObject2D> obj, bool loop = false);

	//����
	void Execute();
private:
	std::vector<SpriteAnimation*> animations;
};