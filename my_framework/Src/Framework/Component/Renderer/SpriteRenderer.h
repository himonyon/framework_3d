#pragma once
/*-----------------------------------------------------------

	SpriteRenderer�R���|�[�l���g
		�X�v���C�g��`�悷��

-------------------------------------------------------------*/

namespace MyFrameWork {
	class SpriteRenderer : public SpriteState {
	private:
		//���_�o�b�t�@
		ID3D11Buffer* pVertexBuffer = 0;

	public:
		SpriteRenderer();
		~SpriteRenderer(void);

		//���_���
		stVertex3D vtx[Sprite::VertexNum];

		//�R���|�[�l���g����
		void Execute() override;

		//�R���|�[�l���g������
		void SetUpSpriteRenderer(noDel_ptr<Sprite> sprite);

		//�F�̐ݒ�A�擾
		void SetColor(float r, float g, float b, float a) override;
		void SetColor(stColor4 color) override;
		stColor4 GetColor() override;

		//�X�v���C�g�̏�Ԃ�������Ԃɖ߂�(UV)
		void SetDefaultUV() override;

	private:
		//�`��
		void Render(void) override;

	};
}