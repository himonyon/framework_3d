#include "../../../framework.h"
#include "../../../environment.h"

ID3D11Device* Direct3D::pDevice = NULL;
ID3D11DeviceContext* Direct3D::pDeviceContext = NULL;
IDXGIDevice* Direct3D::pDXGI;	//DXGI�f�o�C�X
IDXGIAdapter* Direct3D::pAdapter;	//�A�_�v�^
IDXGIFactory* Direct3D::pDXGIFactory;	//�t�@�N�g��
IDXGISwapChain* Direct3D::pSwapChain = NULL;
ID3D11RenderTargetView* Direct3D::pRenderTargetView = NULL;
ID3D11DepthStencilView* Direct3D::pDepthStencilView = NULL;
ID3D11Texture2D* Direct3D::pDepthStencil = NULL;

ID3D11Texture2D* Direct3D::pWhiteTexture = 0;
ID3D11ShaderResourceView* Direct3D::pWhiteTextureView = 0;

bool Direct3D::InitD3D(void* hdl)
{
	HRESULT hr;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;//DirectX11���Direct2D���g�p���邽�߂ɕK�v

	D3D11CreateDevice(NULL,	//IDXGIAdapter *pAdapter : �f�o�C�X�̍쐬���Ɏg�p����r�f�I �A�_�v�^�[�ւ̃|�C���^�[(����̃A�_�v�^�[���g�p����ɂ�NULL)
		D3D_DRIVER_TYPE_HARDWARE,	//D3D_DRIVER_TYPE DriverType : �쐬����f�o�C�X�̎�ނ�\�� D3D_DRIVER_TYPE
		NULL,						//HMODULE Software : �\�t�g�E�F�A ���X�^���C�U�[���������� DLL �̃n���h��
		createDeviceFlags,			//UINT Flags : �L���ɂ��郉���^�C�� ���C���[
		NULL,						//CONST D3D_FEATURE_LEVEL *pFeatureLevels : �쐬�����݂�@�\���x���̏������w�肷�� D3D_FEATURE_LEVEL �̔z��ւ̃|�C���^�[
		0,							//UINT FeatureLevels : pFeatureLevels �̗v�f��
		D3D11_SDK_VERSION,			//UINT SDKVersion : SDK �̃o�[�W����
		&pDevice,					//ID3D11Device **ppDevice : �쐬���ꂽ�f�o�C�X��\�� ID3D11Device �I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ��܂��B
		NULL,						//D3D_FEATURE_LEVEL *pFeatureLevel : ���������ꍇ�́A�������� pFeatureLevels �z��̍ŏ��� D3D_FEATURE_LEVEL ��Ԃ��܂��B���s�����ꍇ�� 0 ��Ԃ��܂��B
		&pDeviceContext				//ID3D11DeviceContext **ppImmediateContext : �f�o�C�X �R���e�L�X�g��\�� ID3D11DeviceContext �I�u�W�F�N�g�ւ̃|�C���^�[�̃A�h���X��Ԃ��܂��B
	);

	//�C���^�[�t�F�C�X�擾
	if (FAILED(pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGI)))
	{
		DestroyD3D();
		return 0;
	}

	//�A�_�v�^�[�擾
	if (FAILED(pDXGI->GetAdapter(&pAdapter)))
	{
		DestroyD3D();
		return 0;
	}

	//�t�@�N�g���[�擾
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);
	if (pDXGIFactory == NULL)
	{
		DestroyD3D();
		return 0;
	}

	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
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

	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	ID3D11Texture2D* pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) {
		return FALSE;
	}
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	
	SAFE_RELEASE(pBackBuffer);
	//�[�x�X�e���V���r���[�̍쐬
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


	//���n(��)�̃e�N�X�`�����쐬
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
	//��ʃN���A�i���ۂ͒P�F�ŉ�ʂ�h��Ԃ������j
	float ClearColor[4] = { 0,0,1,1 };// �N���A�F�쐬�@RGBA�̏�
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);//��ʃN���A
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A
	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
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
