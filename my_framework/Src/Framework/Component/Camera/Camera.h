/*-----------------------------------------------------------

	Camera
		�J�����Ǘ�(�I�u�W�F�N�g�̃X�N���[���ʂ̊Ǘ�)

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