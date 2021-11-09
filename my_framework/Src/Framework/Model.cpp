#include "../../environment.h"
#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")


_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT3& vec)
{
	return DirectX::XMLoadFloat3(&vec);
}

//頂点インプットレイアウトを定義	
D3D11_INPUT_ELEMENT_DESC hInElementDesc_Model[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ID3D11InputLayout* Model:: pVertexLayout = NULL;
ID3D11RasterizerState* Model::pRasterizerState = NULL;
ID3D11BlendState* Model:: pBlendState = NULL;
ID3D11DepthStencilState* Model::pDepthStencilState = NULL;
ID3D11Buffer* Model::pConstantBuffer0 = NULL;
ID3D11Buffer* Model::pConstantBuffer1 = NULL;
ID3D11SamplerState* Model::pSampleLinear = NULL;

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
	//ラスタライザのセット
	Direct3D::getDeviceContext()->RSSetState(pRasterizerState);

	//ブレンドステートの設定
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;

	//ブレンドステートの作成
	Direct3D::getDevice()->CreateBlendState(&BlendDesc, &pBlendState);
	//ブレンドステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetBlendState(pBlendState, NULL, 0xffffffff);

	//深度ステンシルステートの設定
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//深度ステンシルステートの作成
	Direct3D::getDevice()->CreateDepthStencilState(&dsDesc, &pDepthStencilState);
	//深度ステンシルステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetDepthStencilState(pDepthStencilState, 0);

	//頂点インプットレイアウトを作成
	Shader::VertexShader* vs = Shader::getVertexShader(Shader::eVertexShader::VS_3D);
	UINT numElements = sizeof(hInElementDesc_Model) / sizeof(hInElementDesc_Model[0]);
	if (FAILED(Direct3D::getDevice()->CreateInputLayout(hInElementDesc_Model, numElements, vs->getCode(), vs->getLength(), &pVertexLayout)))
	{
		return FALSE;
	}

	//コンスタントバッファー作成(変換行列用)
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(Direct3D::getDevice()->CreateBuffer(&cb, NULL, &pConstantBuffer0)))
	{
		return false;
	}
	//コンスタントバッファー作成(マテリアル用)
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(Direct3D::getDevice()->CreateBuffer(&cb, NULL, &pConstantBuffer1)))
	{
		return false;
	}

	// サンプラステートの設定
	D3D11_SAMPLER_DESC smpDesc;
	memset(&smpDesc, 0, sizeof(smpDesc));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	// サンプラステートの生成
	Direct3D::getDevice()->CreateSamplerState(&smpDesc, &pSampleLinear);

	return true;
}

void Model::DestroyStaticMesh() {
	SAFE_RELEASE(pSampleLinear);
	SAFE_RELEASE(pConstantBuffer0);
	SAFE_RELEASE(pConstantBuffer1);
	SAFE_RELEASE(pVertexLayout);
	SAFE_RELEASE(pDepthStencilState);
	SAFE_RELEASE(pBlendState);
	SAFE_RELEASE(pRasterizerState);
}

Model::Model(const WCHAR* fileName) {
	pVertexBuffer = NULL;
	ppIndexBuffer = NULL;

	//座標読み取り用
	float _x, _y, _z;
	int _v1 = 0, _v2 = 0, _v3 = 0;
	int _vn1 = 0, _vn2 = 0, _vn3 = 0;
	int _vt1 = 0, _vt2 = 0, _vt3 = 0;

	DWORD _dwVertexNum = 0; //頂点数格納
	DWORD _dwFaceNum = 0; //面の数格納

	DWORD _dwVCount = 0;//読み込みカウンター
	DWORD _dwVNCount = 0;//読み込みカウンター
	DWORD _dwVTCount = 0;//読み込みカウンター
	DWORD _dwFCount = 0;//読み込みカウンター

	char _key[256] = { 0 };
	//ファイルを開いて内容を読み込む
	FILE* fp = NULL;
	_wfopen_s(&fp, fileName, L"rt");
	if (fp == NULL) return;

	//まずは頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", _key, (int)sizeof(_key));
		//マテリアル読み込み
		if (strcmp(_key, "mtllib") == 0)
		{
			fscanf_s(fp, "%s ", _key, (int)sizeof(_key));
			wchar_t wKey[256];
			mbstowcs_s(NULL, wKey, _key, (int)sizeof(_key));
			LoadMaterial((LPWSTR)wKey, &pMaterial);
		}
		//頂点
		if (strcmp(_key, "v") == 0)
		{
			_dwVertexNum++;
		}
		//法線
		if (strcmp(_key, "vn") == 0)
		{
			_dwVNCount++;
		}
		//テクスチャー座標
		if (strcmp(_key, "vt") == 0)
		{
			_dwVTCount++;
		}
		//フェイス（ポリゴン）
		if (strcmp(_key, "f") == 0)
		{
			_dwFaceNum++;
		}
	}
	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	int _indexNum = _dwFaceNum * 3;
	stVertex3D* _pVertexBuffer = new stVertex3D[_indexNum];
	XMFLOAT3* _pCoord = new XMFLOAT3[_dwVertexNum];
	for (int i = 0; i < sizeof(*_pCoord) / sizeof(_pCoord[0]); i++) _pCoord[i] = { 0,0,0 };
	XMFLOAT3* _pNormal = new XMFLOAT3[_dwVNCount];
	for (int i = 0; i < sizeof(*_pNormal) / sizeof(_pNormal[0]); i++) _pNormal[i] = { 0,0,0 };
	XMFLOAT2* _pTexture = new XMFLOAT2[_dwVTCount];
	for (int i = 0; i < sizeof(*_pTexture) / sizeof(_pTexture[0]); i++) _pTexture[i] = { 0,0 };

	int _norNum = (int)_dwVNCount;
	int _texNum = (int)_dwVTCount;

	//本読み込み	
	fseek(fp,SEEK_SET,0);
	_dwVCount = 0;
	_dwVNCount = 0;
	_dwVTCount = 0;
	_dwFCount = 0;
	
	while(!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(_key,sizeof(_key));
		fscanf_s(fp,"%s ",_key, (int)sizeof(_key));
		
		//頂点 読み込み
		if(strcmp(_key,"v")==0)
		{		
			fscanf_s(fp,"%f %f %f",&_x,&_y,&_z);
			if ((int)_dwVCount >= _dwVertexNum) continue;
			_pCoord[_dwVCount].x=-_x;//OBJは右手座標系なのでxあるいはｚを反転
			_pCoord[_dwVCount].y=_y;
			_pCoord[_dwVCount].z=_z;
			_dwVCount++;
		}
		//法線読み込み
		if (strcmp(_key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &_x, &_y, &_z);
			if ((int)_dwVNCount >= _norNum) continue;
			_pNormal[_dwVNCount].x = -_x;//OBJは右手座標系なのでxあるいはｚを反転
			_pNormal[_dwVNCount].y = _y;
			_pNormal[_dwVNCount].z = _z;
			_dwVNCount++;
		}
		//テクスチャー座標 読み込み
		if (strcmp(_key, "vt") == 0)
		{
			fscanf_s(fp, "%f %f", &_x, &_y);
			if ((int)_dwVTCount >= _texNum) continue;
			_pTexture[_dwVTCount].x = _x;
			_pTexture[_dwVTCount].y = 1 - _y;//OBJファイルはY成分が逆なので合わせる
			_dwVTCount++;
		}
	}

	//マテリアルの数だけインデックスバッファーを作成
	ppIndexBuffer = new ID3D11Buffer * [materialNum];

	//フェイス読み込み バラバラに収録されている可能性があるので、マテリアル名を頼りにつなぎ合わせる
	bool _flag = false;
	int* _pFaceBuffer = new int[_indexNum];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)
	_dwFCount = 0;
	DWORD _dwPartFCount = 0;
	for (int i = 0; i < materialNum; i++)
	{
		_dwPartFCount = 0;
		fseek(fp, SEEK_SET, 0);

		while (!feof(fp))
		{
			//キーワード 読み込み
			ZeroMemory(_key, sizeof(_key));
			fscanf_s(fp, "%s ", _key, (int)sizeof(_key));

			//フェイス 読み込み→頂点インデックスに
			if (strcmp(_key, "usemtl") == 0)
			{
				fscanf_s(fp, "%s ", _key, (int)sizeof(_key));
				if (strcmp(_key, pMaterial[i].name) == 0)
				{
					_flag = true;
				}
				else
				{
					_flag = false;
				}
			}
			if (strcmp(_key, "f") == 0 && _flag == true)
			{
				if (pMaterial[i].pTexture != NULL)//テクスチャーありサーフェイス
				{
					fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &_v1, &_vt1, &_vn1, &_v2, &_vt2, &_vn2, &_v3, &_vt3, &_vn3);
				}
				else//テクスチャー無しサーフェイス
				{
					fscanf_s(fp, "%d//%d %d//%d %d//%d", &_v1, &_vn1, &_v2, &_vn2, &_v3, &_vn3);
				}

				//インデックスバッファー
				_pFaceBuffer[_dwPartFCount * 3] = _dwFCount * 3;
				_pFaceBuffer[_dwPartFCount * 3 + 1] = _dwFCount * 3 + 1;
				_pFaceBuffer[_dwPartFCount * 3 + 2] = _dwFCount * 3 + 2;
				//頂点構造体に代入
				_pVertexBuffer[_dwFCount * 3].pos = _pCoord[_v1 - 1];
				_pVertexBuffer[_dwFCount * 3].nor = _pNormal[_vn1 - 1];
				_pVertexBuffer[_dwFCount * 3].tex = _pTexture[_vt1 - 1];
				_pVertexBuffer[_dwFCount * 3 + 1].pos = _pCoord[_v2 - 1];
				_pVertexBuffer[_dwFCount * 3 + 1].nor = _pNormal[_vn2 - 1];
				_pVertexBuffer[_dwFCount * 3 + 1].tex = _pTexture[_vt2 - 1];
				_pVertexBuffer[_dwFCount * 3 + 2].pos = _pCoord[_v3 - 1];
				_pVertexBuffer[_dwFCount * 3 + 2].nor = _pNormal[_vn3 - 1];
				_pVertexBuffer[_dwFCount * 3 + 2].tex = _pTexture[_vt3 - 1];

				_dwPartFCount++;
				_dwFCount++;

			}
		}
		if (_dwPartFCount == 0)//使用されていないマテリアル対策
		{
			ppIndexBuffer[i] = NULL;
			continue;
		}

		//インデックスバッファーを作成
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * _dwPartFCount * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = _pFaceBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &ppIndexBuffer[i]);

		pMaterial[i].faceNum = _dwPartFCount; //マテリアルの面の数をセット
	}
	delete[] _pFaceBuffer;

	fclose(fp);

	//バーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(stVertex3D) * _indexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = _pVertexBuffer;
	Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);

	//一時的な入れ物はdelete
	delete[] _pCoord;
	delete[] _pNormal;
	if(_dwVTCount != 0) delete[] _pTexture;
	delete[] _pVertexBuffer;
}

Model::~Model() {
	delete[] pMaterial;
	SAFE_RELEASE(pVertexBuffer);
	delete[] ppIndexBuffer;
}

void Model::Render() {
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProj;
	
	mWorld = XMMatrixIdentity();
	mView = XMMatrixIdentity();
	mProj = XMMatrixIdentity();

	// <summary>
	//移動・回転・スケーリングの設定
	// </summary>
	//スケーリング
	XMMATRIX mScale = GetScaleMatrix();
	mWorld = mWorld * mScale;//ワールド行列へ反映
	//回転
	XMMATRIX mRot = GetRotMatrix();
	mWorld = mWorld * mRot;
	//移動
	XMMATRIX mPos = GetPosMatrix();
	mWorld = mWorld * mPos; 

	// ビュートランスフォーム（視点座標変換）
	XMFLOAT3 _EyePt(0.0f, 0.0f, -0.5f); //カメラ（視点）位置
	XMFLOAT3 _LookatPt(0.0f, 0.0f, 0.0f);//注視位置
	XMFLOAT3 _UpVec(0.0f, 1.0f, 0.0f);//上方位置
	mView = XMMatrixLookAtLH(ToXMVECTOR(_EyePt), ToXMVECTOR(_LookatPt), ToXMVECTOR(_UpVec));
	// プロジェクショントランスフォーム（射影変換）
	mProj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);


	//使用するシェーダーの登録	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), NULL, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBuffer0 cb;
		//ワールド行列を渡す
		cb.mW = mWorld;
		cb.mW = XMMatrixTranspose(cb.mW);
		//ワールド、カメラ、射影行列を渡す
		XMMATRIX m = mWorld * mView * mProj;
		cb.mWVP = m;
		cb.mWVP = XMMatrixTranspose(cb.mWVP);
		//ライトの方向を渡す
		XMFLOAT3 vLightDir(0, 0, 0);
		cb.lightDir = XMFLOAT4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);
		//視点位置を渡す
		cb.eye = XMFLOAT4(_EyePt.x, _EyePt.y, _EyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBuffer0));
		Direct3D::getDeviceContext()->Unmap(pConstantBuffer0, 0);
	}
	//このコンスタントバッファーを使うシェーダーの登録
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer0);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &pConstantBuffer0);
	//頂点インプットレイアウトをセット
	Direct3D::getDeviceContext()->IASetInputLayout(pVertexLayout);
	//プリミティブ・トポロジーをセット
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	UINT stride = sizeof(stVertex3D);
	UINT offset = 0;
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (int i = 0; i < materialNum; i++)
	{
		//使用されていないマテリアル対策
		if (pMaterial[i].faceNum == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		Direct3D::getDeviceContext()->IASetIndexBuffer(ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			ConstantBuffer1 cb;
			cb.ambient = pMaterial[i].ambient;//アンビエントををシェーダーに渡す
			cb.diffuse = pMaterial[i].diffuse;//ディフューズカラーをシェーダーに渡す
			cb.specular = pMaterial[i].specular;//スペキュラーをシェーダーに渡す
			memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBuffer1));
			Direct3D::getDeviceContext()->Unmap(pConstantBuffer1, 0);
		}
		Direct3D::getDeviceContext()->VSSetConstantBuffers(1, 1, &pConstantBuffer1);
		Direct3D::getDeviceContext()->PSSetConstantBuffers(1, 1, &pConstantBuffer1);
		
		//テクスチャーをシェーダーに渡す
		if (pMaterial[i].textureName[0] != NULL)
		{
			Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSampleLinear);
			Direct3D::getDeviceContext()->PSSetShaderResources(0, 1, &pMaterial[i].pTexture);
		}
		//プリミティブをレンダリング
		Direct3D::getDeviceContext()->DrawIndexed(pMaterial[i].faceNum * 3, 0, 0);
	}
}

bool Model::LoadMaterial(LPWSTR fileName, stMaterial** ppMaterial) {
	//マテリアルファイルを開いて内容を読み込む
	FILE* fp = NULL;
	wchar_t pass[256] = L"Data/Object/";
	wcscat_s(pass, fileName);

	_wfopen_s(&fp, pass, L"rt");
	if (fp == NULL) return false;
	char key[110] = { 0 };
	XMFLOAT4 v(0, 0, 0, 0);

	//マテリアル数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, (int)sizeof(key));
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			materialNum++;
		}
	}
	stMaterial* pMaterial = new stMaterial[materialNum];

	//読み込み
	fseek(fp, SEEK_SET, 0);
	int iMCount = -1;

	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, (int)sizeof(key));
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			iMCount++;
			if (iMCount >= materialNum) break;
			fscanf_s(fp, "%s ", key, (int)sizeof(key));
			strcpy_s(pMaterial[iMCount].name, key);
		}
		//Ka　アンビエント
		if (strcmp(key, "Ka") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].ambient = v;
		}
		//Kd　ディフューズ
		if (strcmp(key, "Kd") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].diffuse = v;
		}
		//Ks　スペキュラー
		if (strcmp(key, "Ks") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].specular = v;
		}
		//map_Kd　テクスチャー
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &pMaterial[iMCount].textureName, (int)sizeof(pMaterial[iMCount].textureName));
			wchar_t wTextureName[256];
			mbstowcs_s(NULL, wTextureName, pMaterial[iMCount].textureName, (int)sizeof(pMaterial[iMCount].textureName));
			wchar_t iPass[256] = L"Data/Image/";
			wcscat_s(iPass, wTextureName);
			//テクスチャー作成
			TexMetadata imagedata;
			auto image = std::make_unique<ScratchImage>();
			if (FAILED(LoadFromWICFile(iPass, WIC_FLAGS_NONE, &imagedata, *image)))
			{
				return false;
			}
			if (FAILED(CreateShaderResourceView(Direct3D::getDevice(), image->GetImages(), image->GetImageCount(), imagedata, &pMaterial[iMCount].pTexture)))
			{
				// 失敗
				return false;
			}
		}
	}
	fclose(fp);

	*ppMaterial = pMaterial;

	return true;
}

XMMATRIX Model::GetPosMatrix() {
	//現在の座標を頂点座標にセット
	return XMMatrixTranslation(-posX, posY, posZ);
}

XMMATRIX Model::GetRotMatrix() {
	XMMATRIX mPitch, mHeading, mBank;//回転行列用
	mPitch = XMMatrixRotationX(-rotX);
	mHeading = XMMatrixRotationY(rotY);
	mBank = XMMatrixRotationZ(rotZ);
	return mPitch * mHeading * mBank;
}

XMMATRIX Model::GetScaleMatrix() {
	return XMMatrixScaling(scaleX, scaleY, scaleZ);
}