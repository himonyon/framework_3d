/*-----------------------------------------------------------

	Direct3Dクラス
		DirectXの描画処理の初期化

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {

	class Direct3D {
	public:
		static bool InitD3D(void* hdl);
		static void DestroyD3D();

		static void Clear();

		static ID3D11Device* GetDevice(void);
		static ID3D11DeviceContext* GetDeviceContext(void);
		static IDXGISwapChain* GetSwapChain(void);
		static ID3D11RenderTargetView* GetRenderTargetView(void);
		static ID3D11DepthStencilView* GetDepthStencilView(void);
		static void SetShaderResource(ID3D11ShaderResourceView* p, int slot = 0);
	public:
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
}