/*-----------------------------------------------------------

	スプライトクラス
		画像の情報（頂点情報）を保有

-------------------------------------------------------------*/
#pragma once

class Sprite {
public:
	const static int VertexNum = 4;

private:
	stVertex2D vtx[4] = {};

private:
	//頂点バッファ
	ID3D11Buffer* pVertexBuffer = 0;

	//テクスチャ
	ID3D11Resource* pTexture  =0;
	ID3D11ShaderResourceView* pTextureView = 0;

public:
	Sprite(const WCHAR* sprite_file, const WCHAR* sprite_name = L"default");
	~Sprite(void);

	stVertex2D GetVertexState(int index) { return vtx[index]; }

	ID3D11Resource* GetTexture() { return pTexture; }
	ID3D11ShaderResourceView* GetTextureView() { return pTextureView; }
	ID3D11Buffer* GetPVertexBuffer() { return pVertexBuffer; }
	ID3D11Buffer** GetPPVertexBuffer() { return &pVertexBuffer; } //ポインタのアドレス取得
};

