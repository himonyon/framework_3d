#pragma once
/*-----------------------------------------------------------

	�Q�[���I�u�W�F�N�g�N���X
		2D,3D�I�u�W�F�N�g�̋��ʂ̊Ǘ�

-------------------------------------------------------------*/


class GameObject : public Behavior {
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
	//�I�u�W�F�N�g�̏����V�[��(eSceneTable)
	int belongSceneType = 0;

	//�O�t���[���ł̍��W
	stVector3 before_position;
	//�O�t���[���ł̃X�P�[��
	stVector3 before_scale;
	//�O�t���[���ł̉�](���W�A��)
	stVector3 before_rot;

public:
	GameObject(float x, float y, float z, bool isRender = true, noDel_ptr<GameObject> parent = nullptr);
	~GameObject(void);

	virtual void Render(void); //�`�揈��

	//�I�u�W�F�N�g�̍��W�A��]�A�X�P�[���̍X�V
	void UpdateObjState();

	void Start() {};
	void Update() {};

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

private:
	//�e�I�u�W�F�N�g�ɑ΂��Ďq�I�u�W�F�N�g�̍��W�A��]�A�X�P�[���𑊑ΓI�ɐݒ�
	void SetRelativeState();
};
