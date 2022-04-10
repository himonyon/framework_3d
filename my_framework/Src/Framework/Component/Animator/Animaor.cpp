#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

Animator::Animator() : Component(eComponentType::Animator) {
}
Animator::~Animator() {
	for (auto& box : animBoxes) {
		delete box;
	}
}

void Animator::Execute() {
	//アニメーション処理
	for (auto& box : animBoxes) {
		if (box->trigger) {
			box->anim->AnimOn();
			if (box->anim->IsEnd() && box->anim->IsLoop() == false) {
				box->trigger = false;
			}
		}
	}
}

void Animator::AddAnimation(std::string name, SpriteAnimation* anim) {
	AnimBox* animBox = new AnimBox(name, anim);
	///アニメーションさせる対象設定
	animBox->anim->SetAnimRenderer(gameObject->GetComponent<SpriteState>());
	animBoxes.emplace_back(animBox);
}

void Animator::PlayAnim(std::string name) {
	for (auto& box : animBoxes) {
		if (box->name == name) {
			box->trigger = true;
			return;
		}
	}
}
bool Animator::IsPlayAnim(std::string name) {
	for (auto& box : animBoxes) {
		if (box->name == name) {
			if(box->trigger) return true;
			break;
		}
	}
	return false;
}
void Animator::StopAnim() {
	for (auto& box : animBoxes) {
		box->trigger = false;
	}
}
void Animator::StopAnim(std::string name) {
	for (auto& box : animBoxes) {
		if (box->name == name) {
			box->trigger = false;
			return;
		}
	}
}


///ーーーーーーーーーーーーーーーーーーーーーーーー
/// AnimBoxクラス
/// ーーーーーーーーーーーーーーーーーーーーーーー
Animator::AnimBox::AnimBox(std::string name, SpriteAnimation* anim){
	this->name = name;
	this->anim = anim;
}
Animator::AnimBox::~AnimBox() {
	delete anim;
}




