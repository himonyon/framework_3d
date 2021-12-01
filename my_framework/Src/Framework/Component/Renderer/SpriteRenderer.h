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
	float sizeX;
	float sizeY;

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

private:
	int renderPriority = 0; //�`�揇
	bool sortSwitch = true; //�`�揇���ύX���ꂽ���ǂ���

public:
	static bool Initialize();
	static void Destroy();

	SpriteRenderer(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);
	~SpriteRenderer(void);

	//�R���|�[�l���g����
	void Execute() override;

	void SetSize(float width, float height);
	void SetColor(float r, float g, float b, float a);
	void SetColor(stColor4 color);

	void SetDefaultState(); //�X�v���C�g�̏�Ԃ�������Ԃɖ߂�(Color��UV)

	void SetRenderPriority(int value); //�`�揇�ʂ̐ݒ�
	int GetRenderPriority(); //�`�揇�ʂ̏���
	void SetSortSwitch(bool flag);
	bool isSortSwitch(); //�`�揇���ύX���炽��

private:
	void Render(void); //�`��

	void SetVertexState();
};