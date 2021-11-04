#include "../../environment.h"


_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT3& vec)
{
	return DirectX::XMLoadFloat3(&vec);
}

//頂点インプットレイアウトを定義	
D3D11_INPUT_ELEMENT_DESC hInElementDesc_Model[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ID3D11InputLayout* Model:: pVertexLayout = NULL;
ID3D11RasterizerState* Model::pRasterizerState = NULL;
ID3D11Buffer* Model::pConstantBuffer = NULL;

bool Model::InitStaticMesh() {
	//ラスタライザの設定
	D3D11_RASTERIZER_DESC hRasterizerDesc = {
		D3D11_FILL_SOLID,	//フィルモード
		D3D11_CULL_NONE,	//カリングモード
		FALSE,
		0,
		0.0f,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		FALSE
	};

	//ラスタライザの作成
	Direct3D::getDevice()->CreateRasterizerState(&hRasterizerDesc, &pRasterizerState);

	//頂点インプットレイアウトを作成
	Shader::VertexShader* vs = Shader::getVertexShader(Shader::eVertexShader::VS_3D);
	UINT numElements = sizeof(hInElementDesc_Model) / sizeof(hInElementDesc_Model[0]);
	if (FAILED(Direct3D::getDevice()->CreateInputLayout(hInElementDesc_Model, numElements, vs->getCode(), vs->getLength(), &pVertexLayout)))
	{
		return FALSE;
	}

	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D::getDevice()->CreateBuffer(&cb, NULL, &pConstantBuffer)))
	{
		return false;
	}

	return true;
}

void Model::DestroyStaticMesh() {
	SAFE_RELEASE(pConstantBuffer);
	SAFE_RELEASE(pVertexLayout);
	SAFE_RELEASE(pRasterizerState);
}

Model::Model(const WCHAR* fileName) {
	pVertexBuffer = NULL;
	pIndexBuffer = NULL;

	//座標読み取り用
	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vertexNum = 0;
	char key[190] = { 0 };
	//ファイルを開いて内容を読み込む
	FILE* fp = NULL;
	_wfopen_s(&fp, fileName, L"rt");
	if (fp == NULL) return;

	//まずは頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, (int)sizeof(key));
		//頂点
		if (strcmp(key, "v") == 0)
		{
			vertexNum++;
		}
		//フェイス（ポリゴン）
		if (strcmp(key, "f") == 0)
		{
			faceNum++;
		}
	}
	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	XMFLOAT3* pvVertexBuffer = new XMFLOAT3[vertexNum];
	int* piFaceBuffer = new int[faceNum * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)

	//本読み込み	
	fseek(fp,SEEK_SET,0);
	DWORD dwVCount=0;//読み込みカウンター
	DWORD dwFCount=0;//読み込みカウンター
	while(!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key,sizeof(key));
		fscanf_s(fp,"%s ",key, (int)sizeof(key));
		//頂点 読み込み
		if(strcmp(key,"v")==0)
		{		
			fscanf_s(fp,"%f %f %f",&x,&y,&z);
			pvVertexBuffer[dwVCount].x=-x;//OBJは右手座標系なのでxあるいはｚを反転
			pvVertexBuffer[dwVCount].y=y;
			pvVertexBuffer[dwVCount].z=z;
			dwVCount++;
		}
		//フェイス（ポリゴン） 読み込み
		if(strcmp(key,"f")==0)
		{
			fscanf_s(fp,"%d// %d// %d//",&v1,&v2,&v3);
			piFaceBuffer[dwFCount*3]=v1-1;
			piFaceBuffer[dwFCount*3+1]=v2-1;
			piFaceBuffer[dwFCount*3+2]=v3-1;
			dwFCount++;
		}
		
	}

	fclose(fp);

	//バーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(XMFLOAT3) * vertexNum * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;
	if (FAILED(Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer))) {
		return;
	}

	//インデックスバッファーを作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * faceNum * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = piFaceBuffer;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pIndexBuffer))) {
		return;
	}

	//一時的な入れ物は、もはや不要
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return;
}

Model::~Model() {
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pIndexBuffer);
}

void Model::Render() {
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;
	//ワールドトランスフォーム（絶対座標変換）
	world = XMMatrixRotationY(timeGetTime() / 1100.0f);//単純にyaw回転させる
	// ビュートランスフォーム（視点座標変換）
	XMFLOAT3 vEyePt(0.0f, 1.0f, -2.0f); //カメラ（視点）位置
	XMFLOAT3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	XMFLOAT3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	view = XMMatrixLookAtLH(ToXMVECTOR(vEyePt), ToXMVECTOR(vLookatPt), ToXMVECTOR(vUpVec));
	// プロジェクショントランスフォーム（射影変換）
	proj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);

	//使用するシェーダーの登録	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), NULL, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBuffer cb;
	if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		XMMATRIX m = world * view * proj;
		m = XMMatrixTranspose(m);
		cb.WVP = m;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBuffer));
		Direct3D::getDeviceContext()->Unmap(pConstantBuffer, 0);
	}
	//このコンスタントバッファーを使うシェーダーの登録
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &pConstantBuffer);
	//頂点インプットレイアウトをセット
	Direct3D::getDeviceContext()->IASetInputLayout(pVertexLayout);
	//プリミティブ・トポロジーをセット
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::getDeviceContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//プリミティブをレンダリング
	Direct3D::getDeviceContext()->DrawIndexed(faceNum * 3, 0, 0);
}