#include "../../framework.h"
#include "../../environment.h"


AnimationManager::~AnimationManager() {
	for (auto& anim : animations) {
		delete anim;
	}
	animations.clear();
}

noDel_ptr<SpriteAnimation> AnimationManager::CreateAnimation(SpriteAnimation* instance) {
	animations.emplace_back(instance);
	noDel_ptr<SpriteAnimation> p = noDel_ptr<SpriteAnimation>(animations.back());
	return p;
}
noDel_ptr<SpriteAnimation> AnimationManager::CreateAnimation(noDel_ptr<GameObject2D> obj, bool loop)
{
	SpriteAnimation* instance = new SpriteAnimation(obj, loop);
	animations.emplace_back(instance);
	noDel_ptr<SpriteAnimation> p = noDel_ptr<SpriteAnimation>(animations.back());
	return p;
}

void AnimationManager::Execute() {
	//アニメーション処理
	for (auto& anim : animations) {
		if (anim->isAnimEnable()) {
			anim->AnimOn();
		}
	}
}