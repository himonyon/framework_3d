#pragma once
/*-----------------------------------------------------------

	�I�u�W�F�N�g�N���X
		�I�u�W�F�N�g�̋��ʂ̊Ǘ�
		�R���|�[�l���g�̏��L

-------------------------------------------------------------*/

class Object {
private:
	int objectID = 0;

public:
	virtual ~Object() {};

	void SetObjectID(int id) { objectID = id; }

	int GetObjectID() { return objectID; }
};