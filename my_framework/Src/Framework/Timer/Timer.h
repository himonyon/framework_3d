/*-----------------------------------------------------------

	�^�C�}�[�N���X
		�ʂɃ^�C�}�[���g�������Ƃ��ɗ��p

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {
	class Timer {
	private:
		static float frameTime; //�t���[���^�C��
		static float beforeTime; //�O�̃t���[���̎���

		bool flag = false; //�^�C�}�[�t���O

	public:
		float time = 0;

	public:
		//�t���[���^�C���v�Z
		static void FrameTimeExecute();

		void Start();
		void Stop();
		void Resume();
		void Execute();

		//getter
		float GetFrameTime() const { return frameTime; }
	};
}