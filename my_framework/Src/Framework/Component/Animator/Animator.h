/*-----------------------------------------------------------

	Animator�R���|�[�l���g
		�A�j���[�V�����Ǘ�

-------------------------------------------------------------*/

#pragma once

namespace MyFrameWork {
	class Animator : public Component {
	public:
		//�A�j���[�V�����i�[�N���X
		class AnimBox {
		public:
			AnimBox(std::string name, SpriteAnimation* anim);
			~AnimBox();

			bool trigger = false; //�A�j���[�V�����g���K�[
		public:
			std::string name;
			SpriteAnimation* anim;
		};

		//�R���X�g���N�^
		Animator();
		//�f�X�g���N�^
		~Animator();

		//�R���|�[�l���g����
		void Execute() override;

		//�A�j���[�V�����ꗗ�փA�j���[�V������������
		void AddAnimation(std::string name, SpriteAnimation* anim);

		//�A�j���[�V�����J�n
		void PlayAnim(std::string name);
		//�A�j���[�V�������s����
		bool IsPlayAnim(std::string name);
		//�A�j���[�V�����X�g�b�v�i�S�́j
		void StopAnim();
		//�A�j���[�V�����X�g�b�v
		void StopAnim(std::string name);

	private:
		//�A�j���[�V�����ꗗ
		std::vector<AnimBox*> animBoxes;
	};
}