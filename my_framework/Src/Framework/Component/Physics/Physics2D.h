/*-----------------------------------------------------------

	Physics2D
		２Dオブジェクトの物理挙動
		衝突挙動、重力挙動
		
-------------------------------------------------------------*/

#pragma once

class Physics2D : public Component {
public:
	Physics2D();
	~Physics2D() {};

	void Execute() override {};
};