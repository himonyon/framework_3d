/*-----------------------------------------------------------

	Cameraコンポーネント
		カメラ管理(オブジェクトのスクロール量の管理)

-------------------------------------------------------------*/

#pragma once

namespace MyFrameWork {
	class Camera : public Component {
	public:
		static noDel_ptr<Camera> main;

	public:
		Camera();
		~Camera() {};

		//メインカメラのセット
		void SetMain() { main = noDel_ptr<Camera>(this); }
		//コンポーネント処理
		void Execute() override {};
	};
}