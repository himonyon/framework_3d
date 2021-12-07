#pragma once
/*-----------------------------------------------------------

	オブジェクトクラス
		オブジェクトの共通の管理
		コンポーネントの所有

-------------------------------------------------------------*/

class Object {
private:
	static int max_instance;
	int instanceID = 0;

public:
	Object();
	virtual ~Object() {};

	int GetInstanceID() const { return instanceID; }

};