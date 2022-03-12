#pragma once
/*-----------------------------------------------------------

	SpriteRenderer�R���|�[�l���g
		�X�v���C�g��`�悷��

-------------------------------------------------------------*/

namespace MyFrameWork {
	class SpriteRenderer : public Renderer2D {
	private:
		//���_�o�b�t�@
		ID3D11Buffer* pVertexBuffer = 0;
		ID3D11Buffer* pIndexBuffer = 0;

		static const int indexNum = 6;

		static XMMATRIX View;
		static XMMATRIX Proj;

	public:
		stColor4 color = { 1,1,1,1 };

	public:
		SpriteRenderer();
		~SpriteRenderer(void);

		//��ʂ̏㉺���E�̈ʒu���W
		static float WorldWHPos[4];

		//���_���
		stVertex3D vtx[indexNum];

		//�R���|�[�l���g����
		void Execute() override;

		//�R���|�[�l���g������
		void SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

		//�F�̐ݒ�A�擾
		void SetColor(float r, float g, float b, float a) override;
		void SetColor(stColor4 color) override;
		stColor4 GetColor() override;

		//�X�v���C�g�̏�Ԃ�������Ԃɖ߂�(Color��UV)
		void SetDefaultUV() override;

	private:
		//�`��
		void Render(void);

		//���_���̍X�V
		void SetVertexState();

		//���W�A��]�A�X�P�[���̕`����W(�J�����K�p)
		stVector3 GetPosOnCam();
		stVector3 GetRotOnCam();
		stVector3 GetScaleOnCam();

	public:
		static XMVECTOR CalcScreenToWorld(
			XMVECTOR& pout,
			int Sx,
			int Sy,
			float fZ,
			int Screen_w,
			int Screen_h
		);
		// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
		static stVector3 CalcScreenToXZ(
			int Sx,
			int Sy,
			int Screen_w,
			int Screen_h
		);
	};
}