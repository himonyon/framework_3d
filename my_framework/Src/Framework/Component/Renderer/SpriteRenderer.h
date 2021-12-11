#pragma once
/*-----------------------------------------------------------

	SpriteRenderer�N���X
		�X�v���C�g��`�悷��N���X

-------------------------------------------------------------*/

class SpriteRenderer : public Component {
public:
	//�`��ΏۃX�v���C�g
	noDel_ptr<Sprite> pRenderSprite = NULL;

	//�摜�̕�
	float sizeX = 0;
	float sizeY = 0;

	//���_���
	stVertex2D vtx[Sprite::VertexNum];

private:
	static ID3D11Buffer* pConstantBuffer;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11SamplerState* pSamplerState;
	static ID3D11BlendState* pBlendState;
	static ID3D11DepthStencilState* pDepthStencilState;
	static ID3D11InputLayout* pInputLayout;
	static UINT	VertexStrides;
	static UINT	VertexOffsets;

public:
	static bool Initialize();
	static void Destroy();

	SpriteRenderer();
	~SpriteRenderer(void);

	//�R���|�[�l���g�̏�����
	void SetUpSpriteRenderer(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

	//�R���|�[�l���g����
	void Execute() override;

	void SetSize(float width, float height);
	void SetColor(float r, float g, float b, float a);
	void SetColor(stColor4 color);

	void SetDefaultState(); //�X�v���C�g�̏�Ԃ�������Ԃɖ߂�(Color��UV)

	void SetRenderPriority(float value); //�`�揇�ʂ̐ݒ�
	float GetRenderPriority(); //�`�揇�ʂ̏���

private:
	void Render(void); //�`��

	void SetVertexState();
};