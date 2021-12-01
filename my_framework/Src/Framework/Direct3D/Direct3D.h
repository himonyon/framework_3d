/*-----------------------------------------------------------

	Direct3Dクラス
		DirectXの描画処理の初期化

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
	static IDXGIDevice* pDXGI;				//DXGIデバイス
	static IDXGIAdapter* pAdapter;			//アダプタ
	static IDXGIFactory* pDXGIFactory;		//ファクトリ
	static IDXGISwapChain* pSwapChain;
	static ID3D11RenderTargetView* pRenderTargetView;
	static ID3D11DepthStencilView* pDepthStencilView;
	static ID3D11Texture2D* pDepthStencil;

	//テクスチャ
	static ID3D11Texture2D* pWhiteTexture;
	static ID3D11ShaderResourceView* pWhiteTextureView;
};