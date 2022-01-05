#pragma once
/*-----------------------------------------------------------

	SpriteRenderer�N���X
		�X�v���C�g��`�悷��N���X

-------------------------------------------------------------*/

class ImageRenderer : public Renderer2D {
public:
	ImageRenderer();
	~ImageRenderer(void);

	//���_���
	stVertex2D vtx[Sprite::VertexNum];

	//�R���|�[�l���g����
	void Execute() override;

	void SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

	void SetColor(float r, float g, float b, float a) override;
	void SetColor(stColor4 color) override;
	stColor4 GetColor() override;

	void SetDefaultUV(); //�X�v���C�g�̏�Ԃ�������Ԃɖ߂�(Color��UV)

private:
	void Render(void); //�`��

	void SetVertexState();
};