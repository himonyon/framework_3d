/*-----------------------------------------------------------

	Physics2D
		�QD�I�u�W�F�N�g�̕�������
		�Փˋ����A�d�͋���(������)
		
-------------------------------------------------------------*/

#pragma once

namespace MyFrameWork {
	class Physics2D : public Component {
	public:
		noDel_ptr<Collider2D> col;

	public:
		Physics2D();
		~Physics2D() {};

		//�R���|�[�l���g����
		void Execute() override {};
	};
}