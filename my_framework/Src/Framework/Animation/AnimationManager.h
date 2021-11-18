#pragma once

class AnimationManager {
public:
	AnimationManager() {};
	~AnimationManager();

	noDel_ptr<SpriteAnimation> CreateAnimation(SpriteAnimation* instance);
	noDel_ptr<SpriteAnimation> CreateAnimation(noDel_ptr<GameObject2D> obj, bool loop = false);

	//èàóù
	void Execute();
private:
	std::vector<SpriteAnimation*> animations;
};