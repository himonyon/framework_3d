#pragma once
/*-----------------------------------------------------------

	�I�u�W�F�N�g�N���X
		�I�u�W�F�N�g�A�R���|�[�l���g���ʂ̏��

-------------------------------------------------------------*/

namespace MyFrameWork {

	class Object {
	private:
		static int max_instance;
		int instanceID = 0;

	public:
		Object();
		virtual ~Object() {};

		int GetInstanceID() const { return instanceID; }
	};
}