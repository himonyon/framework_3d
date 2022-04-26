#pragma once
/*-----------------------------------------------------------

	ImageRenderer�R���|�[�l���g
		�摜�i�X�N���[�����W�n�j��`�悷��

-------------------------------------------------------------*/
namespace MyFrameWork {
	class ImageRenderer : public SpriteState {
	private:
		stVertex2D vtx[Sprite::VertexNum];

	public:
		ImageRenderer();
		~ImageRenderer(void);

		//�R���|�[�l���g����
		void Execute() override;

		//�`��Ώۉ摜�̐ݒ�
		void SetUpImageRenderer(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

		//�F�̐ݒ�A�擾
		void SetColor(float r, float g, float b, float a) override;
		void SetColor(stColor4 color) override;
		stColor4 GetColor() override;

		void SetDefaultUV() override;

	private:
		//�`��
		void Render(void) override;

		//���_���@�̍X�V
		void SetVertexState();
	};
}