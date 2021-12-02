#include "../../../framework.h"
#include "../../../environment.h"
Sprite::Sprite(const WCHAR* sprite_file, const WCHAR* sprite_name) {
	HRESULT hr;

	WCHAR _texture_file[256] = L"";
	float _left_u = 0, _right_u = 0, _top_v = 0, _bottom_v = 0;
	FILE* fp = NULL;
	WCHAR _key[256] = { 0 };
	bool flag = false;

	_wfopen_s(&fp, sprite_file, L"rt");
	if (fp == NULL) {
		return;
	}

	while (!feof(fp))
	{
		//キーワード読み込み
		fwscanf_s(fp, L"%s", _key, 256);

		if (wcscmp(_key, L"texture") == 0) {
			fwscanf_s(fp, L"%s", _key, 256);
			wcscpy_s(_texture_file, _key);
		}
		if (wcscmp(_key, L"name") == 0) {
			fwscanf_s(fp, L"%s", _key, 256);
			if (wcscmp(_key, sprite_name) == 0) flag = true;
		}
		if (wcscmp(_key, L"uv") == 0) {
			fwscanf_s(fp, L"%f %f %f %f", &_left_u, &_right_u, &_top_v, &_bottom_v);
			if(flag) break;
		}
	}
	fclose(fp);

	///スプライトの情報を設定ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//頂点カラーを設定
	for (int i = 0; i < VertexNum; i++)
	{
		vtx[i].r = 1.0f;
		vtx[i].g = 1.0f;
		vtx[i].b = 1.0f;
		vtx[i].a = 1.0f;
	}

	//uvを設定
	vtx[0].u = _left_u;	vtx[0].v = _top_v;
	vtx[1].u = _right_u;	vtx[1].v = _top_v;
	vtx[2].u = _left_u;	vtx[2].v = _bottom_v;
	vtx[3].u = _right_u;	vtx[3].v = _bottom_v;

	//テクスチャの読み込み
	if (_texture_file != 0) {
		hr = DirectX::CreateWICTextureFromFile(Direct3D::getDevice(), _texture_file, &pTexture, &pTextureView);
		if (hr != S_OK)
		{
			pTexture = NULL;
			pTextureView = NULL;
		}
	}

	//頂点バッファの設定
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(stVertex2D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);
	//サブリソースデータの設定
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vtx;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	if (FAILED(hr))
	{
		assert(false);
	}
}

Sprite::~Sprite() {
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pTextureView);
}


