/*-----------------------------------------------------------

	Direct3D�N���X
		DirectX�̕`�揈���̏�����

-------------------------------------------------------------*/
#pragma once

class Direct3D {
public:
	static bool InitD3D(void* hdl);
	static void DestroyD3D();

	static void Clear();

	static ID3D11Device* getDevice(void);
	static ID3D11DeviceContext* getDeviceContext(void);
	static IDXGISwapChain* getSwapChain(void);
	static ID3D11RenderTargetView* getRenderTargetView(void);
	static ID3D11DepthStencilView* getDepthStencilView(void);
	static void setShaderResource(ID3D11ShaderResourceView* p, int slot = 0);
private:
	static ID3D11Device* pDevice;
	static ID3D11DeviceContext* pDeviceContext;
	static IDXGIDevice* pDXGI;				//DXGI�f�o�C�X
	static IDXGIAdapter* pAdapter;			//�A�_�v�^
	static IDXGIFactory* pDXGIFactory;		//�t�@�N�g��
	static IDXGISwapChain* pSwapChain;
	static ID3D11RenderTargetView* pRenderTargetView;
	static ID3D11DepthStencilView* pDepthStencilView;
	static ID3D11Texture2D* pDepthStencil;

	//�e�N�X�`��
	static ID3D11Texture2D* pWhiteTexture;
	static ID3D11ShaderResourceView* pWhiteTextureView;
};