/*-----------------------------------------------------------

	Camera�R���|�[�l���g
		�J�����Ǘ�(�I�u�W�F�N�g�̃X�N���[���ʂ̊Ǘ�)

-------------------------------------------------------------*/

#pragma once

namespace MyFrameWork {
	class Camera : public Component {
	public:
		static noDel_ptr<Camera> main;

	public:
		Camera();
		~Camera() {};

		//���C���J�����̃Z�b�g
		void SetMain() { main = noDel_ptr<Camera>(this); }
		//�R���|�[�l���g����
		void Execute() override {};
	};
}