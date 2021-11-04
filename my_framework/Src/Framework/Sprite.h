#pragma once

//���_�\����
struct stVertex2D
{
	float x, y;
	float r, g, b, a;
	float u, v;	
};

class Sprite {
public:
	Sprite* pParent;
	
	float posX;
	float posY;

	float sizeX;
	float sizeY;

	float scaleX;
	float scaleY;

	float rot = 0;

	stVertex2D vtx[4];

private:
	int renderPriority; //�`�揇
	bool sortEnable; //�`�揇���ύX���ꂽ���ǂ���
	bool executeEnable; //���s�Ǘ�
	bool renderEnable; //�`��Ǘ�

private:
	ID3D11Buffer* pVertexBuffer;
	//�e�N�X�`��
	ID3D11Resource* pTexture;
	ID3D11ShaderResourceView* pTextureView;

	static ID3D11Buffer* pConstantBuffer;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11SamplerState* pSamplerState;
	static ID3D11BlendState* pBlendState;
	static ID3D11DepthStencilState* pDepthStencilState;
	static ID3D11InputLayout* pInputLayout;
	static UINT	VertexStrides;
	static UINT	VertexOffsets;

	const static int VertexNum = 4;

public:
	static bool Initialize();
	static void Destroy();

	Sprite(float x, float y, float width, float height,  const WCHAR* texture_file, Sprite* parent = NULL, bool isRender = true, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f);
	~Sprite(void);

	void Render(void); //�ʃ����_�[
	void Execute(void);
	
	void SetPosition(float x, float y);
	void SetColor(float r, float g, float b, float a);
	void SetScale(float x, float y);

	void SetRenderPriority(int value); //�`�揇�ʂ̐ݒ�
	int GetRenderPriority(); //�`�揇�ʂ̏���
	void SetSortEnable(bool flag);
	bool isSortEnable(); //�`�揇���ύX���炽��
	void SetRenderEnable(bool flag); //�`���Ԃ̐ݒ�
	bool isRenderEnable(); //�`���ԂɂȂ��Ă��邩
	void SetExecuteEnable(bool flag); //���s��Ԃ̐ݒ�
	bool isExecuteEnable(); //���s��ԂɂȂ��Ă��邩�ǂ���

	static void TextureCopy(Sprite* copy_to, Sprite* copy_from);

private:
	void SetVertexPosition();
};