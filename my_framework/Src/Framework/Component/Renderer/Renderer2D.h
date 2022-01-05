#pragma once
/*-----------------------------------------------------------

	SpriteRenderer�N���X
		�X�v���C�g��`�悷��N���X

-------------------------------------------------------------*/

class Renderer2D : public Component {
private:
	static D3D11_INPUT_ELEMENT_DESC hInElementDesc_Sprite[];
	static D3D11_INPUT_ELEMENT_DESC hInElementDesc_Model[];

public:
	//�`��ΏۃX�v���C�g
	noDel_ptr<Sprite> pRenderSprite = NULL;

	//�摜�̕�
	float sizeX = 0;
	float sizeY = 0;


protected:
	static ID3D11Buffer* pConstantBuffer; //Image�p
	static ID3D11Buffer* pConstantBuffer0; //Sprite�s��p
	static ID3D11Buffer* pConstantBuffer1; //Sprite�}�e���A���p
	static ID3D11Buffer* pIndexBuffer;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11SamplerState* pSamplerState;
	static ID3D11BlendState* pBlendState;
	static ID3D11DepthStencilState* pDepthStencilState;
	static ID3D11InputLayout* pInputLayout; //Image�p
	static ID3D11InputLayout* pInputLayout1; //Sprite�p
	static UINT	VertexStrides;
	static UINT	VertexOffsets;

public:
	static bool Initialize();
	static void Destroy();

	Renderer2D();
	~Renderer2D(void);

	void SetSize(float width, float height);

	void SetRenderPriority(float value); //�`�揇�ʂ̐ݒ�
	float GetRenderPriority(); //�`�揇�ʂ̏���

	virtual void SetColor(float r, float g, float b, float a) {};
	virtual void SetColor(stColor4 color) {};
	virtual stColor4 GetColor() { return { 0,0,0,0 }; };

private:
	virtual void Render(void) {}; //�`��
};