#pragma once

class GameObject2D : public GameObject {
public:
	//�`��ΏۃX�v���C�g
	noDel_ptr<Sprite> pRenderSprite = NULL;

	//�摜�̕�
	float sizeX;
	float sizeY;

	//�J���[
	float col_r[Sprite::VertexNum], col_g[Sprite::VertexNum], col_b[Sprite::VertexNum], col_a[Sprite::VertexNum];

private:
	int renderPriority = 0; //�`�揇
	bool sortEnable = true; //�`�揇���ύX���ꂽ���ǂ���

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

	GameObject2D(float x, float y, float width, float height, noDel_ptr<Sprite> sprite, bool isRender = true, noDel_ptr<GameObject> parent = NULL);
	~GameObject2D(void);

	void Render(void) override; //�`�揈��
	void Execute(void) override; //����

	void SetColor(float r, float g, float b, float a);
	void SetRotation(float rot);

	void SetRenderPriority(int value); //�`�揇�ʂ̐ݒ�
	int GetRenderPriority(); //�`�揇�ʂ̏���
	void SetSortEnable(bool flag);
	bool isSortEnable(); //�`�揇���ύX���炽��

private:
	void SetVertexState();
};
