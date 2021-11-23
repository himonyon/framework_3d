#pragma once

class GameObject3D : public GameObject {
public:
	//描画対象メッシュ
	noDel_ptr<Mesh> pRenderMesh = NULL;

private:
	static ID3D11InputLayout* pVertexLayout;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11BlendState* pBlendState;
	static ID3D11DepthStencilState* pDepthStencilState;
	static ID3D11Buffer* pConstantBuffer0; //行列用
	static ID3D11Buffer* pConstantBuffer1; //マテリアル用
	static ID3D11SamplerState* pSampleLinear;//テクスチャーのサンプラー

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
