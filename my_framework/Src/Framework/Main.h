#pragma once
#include "../../framework.h"
#include "../../environment.h"

namespace MyFrameWork {

	class Main {
	public:
		bool Init(void* hWnd);
		void Destroy();
		void App();
		void Execute();
		void Render();
	};
}

