/*-----------------------------------------------------------

	�X�v���C�g�N���X
		�摜�̏��i���_���j��ۗL

-------------------------------------------------------------*/
#pragma once

class Sprite {
public:
	const static int VertexNum = 4;

private:
	stVertex2D vtx[4] = {};

private:
	//���_�o�b�t�@
	ID3D11Buffer* pVertexBuffer = 0;

	//�e�N�X�`��
	ID3D11Resource* pTexture  =0;

public:
	ID3D11ShaderResourceView* pTextureView = 0;
	Sprite(const WCHAR* sprite_file, const WCHAR* sprite_name = L"default");
	~Sprite(void);

	stVertex2D GetVertexState(int index) { return vtx[index]; }

	ID3D11Resource* GetTexture() { return pTexture; }
	ID3D11ShaderResourceView* GetTextureView() { return pTextureView; }
	ID3D11Buffer* GetPVertexBuffer() { return pVertexBuffer; }
	ID3D11Buffer** GetPPVertexBuffer() { return &pVertexBuffer; } //�|�C���^�̃A�h���X�擾
};

