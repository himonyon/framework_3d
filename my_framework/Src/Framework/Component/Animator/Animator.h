/*-----------------------------------------------------------

	Animator
		アニメーション管理
		アニメーション実行

-------------------------------------------------------------*/

#pragma once

class Animator : public Component {
public:
	class AnimBox {
	public:
		AnimBox(std::string name, SpriteAnimation* anim);
		~AnimBox();

		bool trigger = false; //アニメーショントリガー
	public:
		std::string name;
		SpriteAnimation* anim;
	};

	Animator();
	~Animator();

	void Execute() override;

	void AddAnimation(std::string name, SpriteAnimation* anim);

	void PlayAnim(std::string name);
	bool IsPlayAnim(std::string name);
	void StopAnim();
	void StopAnim(std::string name);

private:
	std::vector<AnimBox*> animBoxes;
};