#pragma once



class GameObject {
public:

	//���W
	stVector3 position;
	//�X�P�[��
	stVector3 scale;
	//��](���W�A��)
	stVector3 rot;

protected:
	noDel_ptr<GameObject> pParent; //�e�̃I�u�W�F�N�g
	std::vector<noDel_ptr<GameObject>> pChildren; //�q�I�u�W�F�N�g

	bool executeEnable; //���s�Ǘ�
	bool renderEnable; //�`��Ǘ�

public:
	//�O�t���[���ł̍��W
	stVector3 before_position;
	//�O�t���[���ł̃X�P�[��
	stVector3 before_scale;
	//�O�t���[���ł̉�](���W�A��)
	stVector3 before_rot;

public:
	GameObject(float x, float y, float z, bool isRender = true, noDel_ptr<GameObject> parent = nullptr);
	~GameObject(void);

	virtual void Render(void);
	virtual void Execute(void);

	//getter, setter
	noDel_ptr<GameObject> GetParent() { return pParent; }
	void SetParent(noDel_ptr<GameObject> obj);
	void SetPosition(float x, float y);
	void SetScale(float x, float y);
	void SetRotation(float x, float y, float z);
	void SetRenderEnable(bool flag); //�`���Ԃ̐ݒ�
	bool isRenderEnable(); //�`���ԂɂȂ��Ă��邩
	void SetExecuteEnable(bool flag); //���s��Ԃ̐ݒ�
	bool isExecuteEnable(); //���s��ԂɂȂ��Ă��邩�ǂ���
};
