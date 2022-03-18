/*-----------------------------------------------------------

	Physics2D
		２Dオブジェクトの物理挙動
		衝突挙動、重力挙動(未実装)
		
-------------------------------------------------------------*/

#pragma once

namespace MyFrameWork {
	class Physics2D : public Component {
	public:
		noDel_ptr<Collider2D> col;

	public:
		Physics2D();
		~Physics2D() {};

		//コンポーネント処理
		void Execute() override {};
	};
}