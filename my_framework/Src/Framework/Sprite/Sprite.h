/*-----------------------------------------------------------

	スプライトクラス
		画像の情報（頂点情報）を保有

-------------------------------------------------------------*/
#pragma once

class Sprite {
public:
	const static int VertexNum = 4;

	char* name;

private:
	stVertex2D vtx[4];

private:
	//頂点バッファ
	ID3D11Buffer* pVertexBuffer = 0;

	//テクスチャ
	ID3D11Resource* pTexture;
	ID3D11ShaderResourceView* pTextureView;

public:
	Sprite(const WCHAR* texture_file, const char* name, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f);
	~Sprite(void);

	stVertex2D GetVertexState(int index) { return vtx[index]; }

	ID3D11Resource* GetTexture() { return pTexture; }
	ID3D11ShaderResourceView* GetTextureView() { return pTextureView; }
	ID3D11Buffer* GetPVertexBuffer() { return pVertexBuffer; }
	ID3D11Buffer** GetPPVertexBuffer() { return &pVertexBuffer; } //ポインタのアドレス取得
};