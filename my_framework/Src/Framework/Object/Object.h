#pragma once
/*-----------------------------------------------------------

	オブジェクトクラス
		オブジェクトの共通の管理
		コンポーネントの所有

-------------------------------------------------------------*/

class Object {
private:
	int objectID = 0;

public:
	virtual ~Object() {};

	void SetObjectID(int id) { objectID = id; }

	int GetObjectID() { return objectID; }
};