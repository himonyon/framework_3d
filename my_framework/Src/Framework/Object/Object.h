#pragma once
/*-----------------------------------------------------------

	オブジェクトクラス
		オブジェクトの共通の管理
		コンポーネントの所有

-------------------------------------------------------------*/

class Object {
private:
	static int max_instance;
	int objectID = 0;

public:
	Object();//コンポーネント用
	Object(int id); //ゲームオブジェクト用
	virtual ~Object() {};

	int GetObjectID() { return objectID; }

	void SetObjectID(int id) { objectID = id; }

};