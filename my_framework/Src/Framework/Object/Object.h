#pragma once
/*-----------------------------------------------------------

	�I�u�W�F�N�g�N���X
		�I�u�W�F�N�g�̋��ʂ̊Ǘ�
		�R���|�[�l���g�̏��L

-------------------------------------------------------------*/

class Object {
private:
	static int max_instance;
	int objectID = 0;

public:
	Object();//�R���|�[�l���g�p
	Object(int id); //�Q�[���I�u�W�F�N�g�p
	virtual ~Object() {};

	int GetObjectID() { return objectID; }

	void SetObjectID(int id) { objectID = id; }

};