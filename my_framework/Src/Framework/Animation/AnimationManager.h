/*-----------------------------------------------------------

	アニメーション管理クラス
		シーンに作られたアニメーションの管理
		アニメーションクラスインスタンスの所有権を持つ配列を所有

-------------------------------------------------------------*/
#pragma once

class AnimationManager {
public:
	AnimationManager() {};
	~AnimationManager();

	noDel_ptr<SpriteAnimation> CreateAnimation(SpriteAnimation* instance);
	noDel_ptr<SpriteAnimation> CreateAnimation(noDel_ptr<GameObject2D> obj, bool loop = false);

	//処理
	void Execute();
private:
	std::vector<SpriteAnimation*> animations;
};