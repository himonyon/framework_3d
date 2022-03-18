#include "Main.h"

using namespace MyFrameWork;

//固定フレームレートのためのパフォーマンスカウンター
bool performanceCounter;
LARGE_INTEGER freq, startCount, finishCount;
float count;

std::mutex mem_mutex_;

bool Main::Init(void* hWnd) {
	//パフォーマンスカウンタの初期化
	memset(&freq, 0, sizeof(freq));
	memset(&startCount, 0, sizeof(startCount));
	memset(&finishCount, 0, sizeof(finishCount));
	count = 0.0f;

	performanceCounter = QueryPerformanceFrequency(&freq);

	//マウスカーソルを非表示にする
	ShowCursor(false);

	//--------------------------------------------------

	//------------------------------------

	//フレームワーク-------------------------------
	Direct3D::InitD3D(hWnd);
	Font::Initialize(hWnd);
	Shader::InitShader();
	Renderer2D::Initialize();
	Renderer3D::Initialize();
	Sound::InitSound();
	DirectInput::InitInput(hWnd);
	InputConfig::SetUpConfig();

	//シーン作成
	SceneManager::SwitchScene();

	return true;
}

void Main::Destroy() {
	//シーン削除
	SceneManager::DeleteScene();
	//フレームワーク-------------------------------
	InputConfig::DestroyConfig();
	DirectInput::DestroyInput();
	Sound::DestroySound();
	Renderer2D::Destroy();
	Renderer3D::Destroy();
	Shader::DestroyShader();
	Font::Destroy();
	Direct3D::DestroyD3D();
}

void Main::App() {
	if (performanceCounter)
	{	//フレームの開始時間を取得
		QueryPerformanceCounter(&startCount);
	}

	//フレームタイム計測
	Timer::FrameTimeExecute();

	//メイン処理
	Execute();

	//描画クリア
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

	//描画
	Render();

	//シーン切り替え確認
	SceneManager::SwitchScene();

	//フレームレート調整
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

void Main::Execute() {
	DirectInput::KeyManager();

	if(SceneManager::GetMainScene() != nullptr) SceneManager::GetMainScene()->Execute();
}

void Main::Render() {
	if (SceneManager::GetMainScene() != nullptr)SceneManager::GetMainScene()->Render();
}