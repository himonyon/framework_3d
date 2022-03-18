#pragma once
/*-----------------------------------------------------------

	ImageRenderer�R���|�[�l���g
		�摜�i�X�N���[�����W�n�j��`�悷��

-------------------------------------------------------------*/
namespace MyFrameWork {
	class ImageRenderer : public Renderer2D {
	public:
		//���[���h���W�I�u�W�F�N�g�̎�O�����ǂ���ɕ`�悷�邩
		bool isFrontImg = true;

	public:
		ImageRenderer();
		~ImageRenderer(void);

		//���_���
		stVertex2D vtx[Sprite::VertexNum];

		//�R���|�[�l���g����
		void Execute() override;

		//�`��Ώۉ摜�̐ݒ�
		void SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

		//�F�̐ݒ�A�擾
		void SetColor(float r, float g, float b, float a) override;
		void SetColor(stColor4 color) override;
		stColor4 GetColor() override;

		//�`�揃�̐ݒ�FFrontObj�Ȃ�+10000�����l�ABackObj�Ȃ�-10000�����l��Ԃ�
		int GetRenderPriority() override;
		//�X�v���C�g�̏�Ԃ�������Ԃɖ߂�(Color��UV)
		void SetDefaultUV() override;

	private:
		//�`��
		void Render(void);
		//���_���@�̍X�V
		void SetVertexState();
	};
}