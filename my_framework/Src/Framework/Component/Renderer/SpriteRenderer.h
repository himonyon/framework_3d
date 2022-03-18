#pragma once
/*-----------------------------------------------------------

	SpriteRenderer�R���|�[�l���g
		�X�v���C�g��`�悷��

-------------------------------------------------------------*/

namespace MyFrameWork {
	class SpriteRenderer : public Renderer3D {
	private:
		//�`��ΏۃX�v���C�g
		noDel_ptr<Sprite> pRenderSprite = NULL;

		//���_�o�b�t�@
		ID3D11Buffer* pVertexBuffer = 0;

		static const int indexNum = 6;

	public:
		stColor4 color = { 1,1,1,1 };

	public:
		SpriteRenderer();
		~SpriteRenderer(void);

		//���_���
		stVertex3D vtx[indexNum];

		//�R���|�[�l���g����
		void Execute() override;

		//�R���|�[�l���g������
		void SetUpRenderer(noDel_ptr<Sprite> sprite);

		//�F�̐ݒ�A�擾
		void SetColor(float r, float g, float b, float a) override;
		void SetColor(stColor4 color) override;
		stColor4 GetColor() override;

		//�X�v���C�g�̏�Ԃ�������Ԃɖ߂�(Color��UV)
		void SetDefaultUV();

	private:
		//�`��
		void Render(void) override;
	};
}