#pragma once
/*-----------------------------------------------------------

	MeshRenderer�N���X
		���b�V����`�悷��N���X

-------------------------------------------------------------*/

class MeshRenderer : public Component {
private:
	static D3D11_INPUT_ELEMENT_DESC hInElementDesc_Model[];

public:
	//�`��Ώۃ��b�V��
	noDel_ptr<Mesh> pRenderMesh = NULL;

private:
	static ID3D11InputLayout* pVertexLayout;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11BlendState* pBlendState;
	static ID3D11DepthStencilState* pDepthStencilState;
	static ID3D11Buffer* pConstantBuffer0; //�s��p
	static ID3D11Buffer* pConstantBuffer1; //�}�e���A���p
	static ID3D11SamplerState* pSampleLinear;//�e�N�X�`���[�̃T���v���[

public:
	static bool Initialize();
	static void Destroy();

	MeshRenderer();
	~MeshRenderer(void);

	//�R���|�[�l���g�̏�����
	void SetUpMeshRenderer(noDel_ptr<Mesh> mesh);

	//�R���|�[�l���g����
	void Execute() override;

private:
	void Render(void);

	XMMATRIX GetPosMatrix();
	XMMATRIX GetRotMatrix();
	XMMATRIX GetScaleMatrix();
};