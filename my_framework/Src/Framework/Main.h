#pragma once
#include "../../framework.h"
#include "../../environment.h"

class Main {
public:
	bool Init(void* hWnd);
	void Destroy();
	void App();
	void Execute();
	void Render();
};


/// <summary>-------------------------------------------------
///　			スプライトファイル作成用 
/// </summary>----------------------------------------------
//スプライトファイルを作成
static void CreateSpriteFile(const WCHAR* texture_file, float left = 0, float right = 1, float top = 0, float bottom = 1);
//複数保持する場合はこっち
static void AddSpriteFile(const WCHAR* sprite_file, const WCHAR* sprite_name, float left, float right, float top, float bottom);