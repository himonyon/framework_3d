#pragma once
/*-----------------------------------------------------------

	MeshRendererクラス
		メッシュを描画するクラス

-------------------------------------------------------------*/

class MeshRenderer : public Component {
private:
	static D3D11_INPUT_ELEMENT_DESC hInElementDesc_Model[];

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

	MeshRenderer();
	~MeshRenderer(void);

	//コンポーネントの初期化
	void SetUpMeshRenderer(noDel_ptr<Mesh> mesh);

	//コンポーネント処理
	void Execute() override;

private:
	void Render(void);

	XMMATRIX GetPosMatrix();
	XMMATRIX GetRotMatrix();
	XMMATRIX GetScaleMatrix();
};