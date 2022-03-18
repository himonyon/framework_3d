/*-----------------------------------------------------------

	�X�v���C�g�A�j���[�V�����N���X
		�L�[���t���[�����w�肵�Đݒu���A�e�L�[�̎��ԂɂȂ��
		���̃L�[�ɐݒ肳�ꂽ�������s���A�j���[�V�����@�\

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {
	class SpriteAnimation {
	private:
		bool isLoop = false;

		bool isEnd = false;

		float frameCount = 0; //�i�񂾃t���[����

		int curKeyIndex = -1; //���ݓ��B���Ă���L�[�t���[���z��ԍ�

		noDel_ptr<Renderer2D> pAnimRenderer = NULL; //�\������X�v���C�g�����_���[

	private:
		//�A�j���[�V�����̃L�[�t���[���N���X
		class KeyFrame {
		public:
			Sprite* pSprite = 0;
			float frame = 0;
			float x = 0, y = 0;
			stColor4 color = { 1,1,1,1 };
			float scaleX = 1, scaleY = 1;
			float rot = 0;

			//�L�[�ݒ莞�ɕύX���{���ꂽ�v���p�e�B���m�F���邽�߂̃g���K�[
			struct stTrigger {
			public:
				bool scale = false;
				bool rotation = false;
				bool color = false;
			};

			stTrigger trigger;

		public:
			KeyFrame() {};
			~KeyFrame();
		};

	private:
		//�L�[�̔z��
		std::vector<KeyFrame*> keyFrames;

		//�L�[���̃f�t�H���g�l�Z�b�g�A�b�v
		void SetUpDefaultValue(KeyFrame*);

		//�L�[�t���[���̃X�v���C�g��Ԃ̔��f
		void SetKeySpriteState(KeyFrame*);

		//����̃L�[�t���[����Ԃɐݒ�(���W�ȊO)
		void SetKeyState(KeyFrame*);

		//���̃L�[�Ɍ����ĕω��@rate�͕ω���
		void TransPos(KeyFrame* key);
		void TransCol(KeyFrame* key, float rate);
		void TransScl(KeyFrame* key, float rate);
		void TransRot(KeyFrame* key, float rate);

	public:
		//�R���X�g���N�^�F�t�@�C���f�[�^����L�[�z����쐬
		SpriteAnimation(const WCHAR* animation_file, bool loop = false);
		//�f�X�g���N�^
		~SpriteAnimation();

		//�A�j���[�V�������s
		void AnimOn();
		//�A�j���[�V�����I��
		void AnimOff();

		//�A�j���[�V����������Ώۃ����_���[�ݒ�
		void SetAnimRenderer(noDel_ptr<Renderer2D>);

		//Getter,Setter
		bool IsLoop() { return isLoop; }
		bool IsEnd() { return isEnd; }
	};
}
