#pragma once
/*-----------------------------------------------------------

	SpriteState�N���X
		�`�悷��摜�̏��N���X

-------------------------------------------------------------*/

namespace MyFrameWork {
	class SpriteState : public Renderer {
	public:
		noDel_ptr<Sprite> pRenderSprite = NULL;

		stVector2 size = { 1,1 };

	public:
		SpriteState(eComponentType type);
		virtual ~SpriteState();

		//�T�C�Y�ݒ�
		void SetSize(float, float);

		//�X�v���C�g�̏�Ԃ�������Ԃɖ߂�(UV)
		virtual void SetDefaultUV() {};
	};
}