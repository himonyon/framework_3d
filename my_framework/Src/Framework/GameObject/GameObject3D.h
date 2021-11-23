#pragma once

class GameObject3D : public GameObject {
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

	GameObject3D(float x, float y, float z, noDel_ptr<Mesh> mesh, bool isRender = true, noDel_ptr<GameObject> parent = nullptr);
	~GameObject3D(void);

	void Render(void) override;

private:
	XMMATRIX GetPosMatrix();
	XMMATRIX GetRotMatrix();
	XMMATRIX GetScaleMatrix();
};
