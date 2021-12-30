#include "Main.h"

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

	//フレームワーク-------------------------------
	Direct3D::InitD3D(hWnd);
	Font::Initialize(hWnd);
	Shader::InitShader();
	SpriteRenderer::Initialize();
	MeshRenderer::Initialize();
	Sound::InitSound();
	DirectInput::InitInput(hWnd);
	InputConfig::SetUpConfig();

	//シーン作成
	SceneManager::SwitchScene();

	return true;
}
//
//
// 
void Main::Destroy() {

	//フレームワーク-------------------------------
	InputConfig::DestroyConfig();
	DirectInput::DestroyInput();
	Sound::DestroySound();
	SpriteRenderer::Destroy();
	MeshRenderer::Destroy();
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

	SceneManager::SwitchScene();

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

	if(SceneManager::GetMainScene() != nullptr) SceneManager::GetMainScene()->Execute();
}
//
//
//
void Main::Render() {
	if (SceneManager::GetMainScene() != nullptr)SceneManager::GetMainScene()->Render();
}


/// <summary>-------------------------------------------------
///　			スプライトファイル作成用 
/// </summary>----------------------------------------------
void CreateSpriteFile(const WCHAR* texture_file, float left, float right, float top, float bottom) {
	
	WCHAR _filename[256] = L"";
	int _size = (int)wcslen(texture_file);
	int _nameSize = 0;
	for (int i = 0; i < _size; i++) {
		if (texture_file[i] == L'.') break;
		_nameSize++;
	}
	for (int i = 0; i < _nameSize; i++) {
		_filename[i] = texture_file[i];
	}

	wcscat_s(_filename, L".spr");

	FILE* fp = NULL;
	_wfopen_s(&fp, _filename, L"w");
	if (fp == NULL) {
		return;
	}
	fwprintf_s(fp, L"texture %s\n", texture_file);
	fwprintf_s(fp, L"name default uv %f %f %f %f", left, right, top, bottom);

	fclose(fp);
}
void AddSpriteFile(const WCHAR* texture_file, const WCHAR* sprite_name, float left, float right, float top, float bottom) {
	
	WCHAR _filename[256] = L"";
	int _size = (int)wcslen(texture_file);
	int _nameSize = 0;
	for (int i = 0; i < _size; i++) {
		if (texture_file[i] == L'.') break;
		_nameSize++;
	}
	for (int i = 0; i < _nameSize; i++) {
		_filename[i] = texture_file[i];
	}

	wcscat_s(_filename, L".spr");
	FILE* fp = NULL;
	_wfopen_s(&fp, _filename, L"a");
	if (fp == NULL) {
		return;
	}
	fwprintf_s(fp, L"\nname %s uv %f %f %f %f", sprite_name, left, right, top, bottom);

	fclose(fp);
}