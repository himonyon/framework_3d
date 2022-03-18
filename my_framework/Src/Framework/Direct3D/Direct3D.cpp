#include "../../../framework.h"
#include "../../../environment.h"

using namespace MyFrameWork;

ID3D11Device* Direct3D::pDevice = NULL;
ID3D11DeviceContext* Direct3D::pDeviceContext = NULL;
IDXGIDevice* Direct3D::pDXGI;	//DXGIデバイス
IDXGIAdapter* Direct3D::pAdapter;	//アダプタ
IDXGIFactory* Direct3D::pDXGIFactory;	//ファクトリ
IDXGISwapChain* Direct3D::pSwapChain = NULL;
ID3D11RenderTargetView* Direct3D::pRenderTargetView = NULL;
ID3D11DepthStencilView* Direct3D::pDepthStencilView = NULL;
ID3D11Texture2D* Direct3D::pDepthStencil = NULL;

ID3D11Texture2D* Direct3D::pWhiteTexture = 0;
ID3D11ShaderResourceView* Direct3D::pWhiteTextureView = 0;

bool Direct3D::InitD3D(void* hdl)
{
	HRESULT hr;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;//DirectX11上でDirect2Dを使用するために必要

	D3D11CreateDevice(NULL,			//デバイスの作成時に使用するビデオ アダプターへのポインター(既定のアダプターを使用するにはNULL)
		D3D_DRIVER_TYPE_HARDWARE,	//作成するデバイスの種類を表すD3D_DRIVER_TYPE
		NULL,						//ソフトウェア ラスタライザーを実装するDLLのハンドル
		createDeviceFlags,			//有効にするランタイム レイヤー
		NULL,						//作成を試みる機能レベルの順序を指定する D3D_FEATURE_LEVEL の配列へのポインター
		0,							//pFeatureLevels の要素数
		D3D11_SDK_VERSION,			//SDKのバージョン
		&pDevice,					//作成されたデバイスを表す ID3D11Deviceオブジェクトへのポインターのアドレスを返す。
		NULL,						//成功した場合は、成功したpFeatureLevels配列の最初のD3D_FEATURE_LEVELを返す。失敗した場合は 0 を返す。
		&pDeviceContext				//デバイス コンテキストを表すID3D11DeviceContextオブジェクトへのポインターのアドレスを返す。
	);

	//インターフェイス取得
	if (FAILED(pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGI)))
	{
		DestroyD3D();
		return 0;
	}

	//アダプター取得
	if (FAILED(pDXGI->GetAdapter(&pAdapter)))
	{
		DestroyD3D();
		return 0;
	}

	//ファクトリー取得
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);
	if (pDXGIFactory == NULL)
	{
		DestroyD3D();
		return 0;
	}

	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = (HWND)hdl;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	if (FAILED(pDXGIFactory->CreateSwapChain(pDevice, &sd, &pSwapChain)))
	{
		DestroyD3D();
		return 0;
	}

	//レンダーターゲットビューの作成
	ID3D11Texture2D* pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) {
		return FALSE;
	}
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	
	SAFE_RELEASE(pBackBuffer);
	//深度ステンシルビューの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	if (FAILED(hr)) {
		return FALSE;
	}
	hr = pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr)) {
		return FALSE;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.Format = descDepth.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsDesc.Texture2D.MipSlice = 0;
	pDevice->CreateDepthStencilView(pDepthStencil, &dsDesc, &pDepthStencilView);

	//無地(白)のテクスチャを作成
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = 16;
	descDepth.Height = 16;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DYNAMIC;
	descDepth.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	descDepth.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initialData;
	ZeroMemory(&initialData, sizeof(D3D11_SUBRESOURCE_DATA));
	initialData.pSysMem = new BYTE[16 * 16 * 4];
	initialData.SysMemPitch = 16 * 4;
	memset((void*)initialData.pSysMem, 0xff, 16 * 16 * 4);

	hr = pDevice->CreateTexture2D(&descDepth, &initialData, &pWhiteTexture);

	delete[] initialData.pSysMem;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	if (pWhiteTexture != NULL) {
		pDevice->CreateShaderResourceView(pWhiteTexture, &srvDesc, &pWhiteTextureView);
	}

	return TRUE;
}

//
//
//
void Direct3D::DestroyD3D()
{
	SAFE_RELEASE(pWhiteTextureView);
	SAFE_RELEASE(pWhiteTexture);
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pDepthStencil);
	SAFE_RELEASE(pDepthStencilView);
	SAFE_RELEASE(pSwapChain);
	SAFE_RELEASE(pDXGIFactory);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pDXGI);
	SAFE_RELEASE(pDeviceContext);
	SAFE_RELEASE(pDevice);
}
//
//
//
void Direct3D::Clear() {
	//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = { 0,0.5f,1,1 };// クリア色作成　RGBAの順
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);//画面クリア
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア
	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
}
//
//
//
ID3D11Device* Direct3D::getDevice(void) {
	return pDevice;
}
ID3D11DeviceContext* Direct3D::getDeviceContext(void) {
	return pDeviceContext;
}
IDXGISwapChain* Direct3D::getSwapChain(void) {
	return pSwapChain;
}
ID3D11RenderTargetView* Direct3D::getRenderTargetView(void) {
	return pRenderTargetView;
}
ID3D11DepthStencilView* Direct3D::getDepthStencilView(void) {
	return pDepthStencilView;
}

void Direct3D::setShaderResource(ID3D11ShaderResourceView* p, int slot)
{
	ID3D11ShaderResourceView* hpShaderResourceViews[] = { p ? p : pWhiteTextureView };
	Direct3D::getDeviceContext()->PSSetShaderResources(slot, 1, hpShaderResourceViews);
}
