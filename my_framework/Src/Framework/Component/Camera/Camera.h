/*-----------------------------------------------------------

	Camera
		カメラ管理(オブジェクトのスクロール量の管理)

-------------------------------------------------------------*/

#pragma once

class Camera : public Component {
public:
	static noDel_ptr<Camera> main;

public:
	Camera();
	~Camera() {};

	void SetMain() { main = noDel_ptr<Camera>(this); }
	void Execute() override {};
};