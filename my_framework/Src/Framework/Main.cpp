#include "Main.h"

//固定フレームレートのためのパフォーマンスカウンター
bool performanceCounter;
LARGE_INTEGER freq, startCount, finishCount;
float count;

bool Main::Init(void* hWnd) {
	//パフォーマンスカウンタの初期化
	memset(&freq, 0, sizeof(freq));
	memset(&startCount, 0, sizeof(startCount));
	memset(&finishCount, 0, sizeof(finishCount));
	count = 0.0f;

	performanceCounter = QueryPerformanceFrequency(&freq);


	Direct3D::InitD3D(hWnd);
	Font::Initialize(hWnd);
	Shader::InitShader();
	GameObject2D::Initialize();
	GameObject3D::Initialize();
	Sound::InitSound();
	DirectInput::InitInput(hWnd);

	//シーン作成
	switchScene();

	return true;
}
//
//
// 
void Main::Destroy() {
	deleteScene();

	DirectInput::DestroyInput();
	Sound::DestroySound();
	GameObject2D::Destroy();
	GameObject3D::Destroy();
	Shader::DestroyShader();
	Font::Destroy();
	Direct3D::DestroyD3D();
}
//
//
//
void Main::App() {
	if (performanceCounter)
	{	//フレームの開始時間を取得
		QueryPerformanceCounter(&startCount);
	}

	Timer::FrameTimeExecute();

	Execute();

	Direct3D::Clear();

	//ビューポートの設定
	D3D11_VIEWPORT	Viewport;
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = (float)WINDOW_WIDTH;
	Viewport.Height = (float)WINDOW_HEIGHT;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Direct3D::getDeviceContext()->RSSetViewports(1, &Viewport);


	Render();

	switchScene();

	if (performanceCounter)
	{	//フレームの終了時間を取得
		static float frame_msec = 1.0f / 60.0f;
		QueryPerformanceCounter(&finishCount);
		count = ((float)(finishCount.QuadPart - startCount.QuadPart) / (float)freq.QuadPart);

		if (count < frame_msec)
		{	//1/60秒が経つまで待つ
			DWORD wait = (DWORD)((frame_msec - count) * 1000.0f - 0.5f);
			timeBeginPeriod(1);
			Sleep(wait);
			timeEndPeriod(1);
		}
	}

	Direct3D::getSwapChain()->Present(1, 0);
}
//
// 
// 
void Main::Execute() {
	DirectInput::KeyManager();

	pScene->Execute();
}
//
//
//
void Main::Render() {
	pScene->Render();
	Font::Render();
}
