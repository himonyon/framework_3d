#pragma once
/*-----------------------------------------------------------

	Transform�N���X
		���W�A��]�A�X�P�[���ϊ�

-------------------------------------------------------------*/

class Transform : public Component {
public:
	stVector3 position; //���W
	stVector3 rotation; //��]
	stVector3 scale; //�X�P�[��

	//�e���猩�����΍��W
	stVector3 localPosition;
	stVector3 localRotation;
	stVector3 localScale;

	bool isChanged = false; //Transform�ɕύX����������
private:
	noDel_ptr<Transform> pParent = 0; //�e�̃I�u�W�F�N�g
	std::vector<noDel_ptr<Transform>> pChildren; //�q�I�u�W�F�N�g

	stVector3 b_position;
	stVector3 b_rotation;
	stVector3 b_scale;

	stVector3 b_localPosition;
	stVector3 b_localRotation;
	stVector3 b_localScale;
public:
	Transform();
	~Transform();

	//�R���|�[�l���g�̏����ݒ�
	void SetUpTransform(float x, float y, float z, noDel_ptr<Transform> parent);

	//�R���|�[�l���g����
	void Execute() override;

	//�e�̃I�u�W�F�N�g���L����Ԃ��ǂ���
	bool IsParentObjEnable();

	noDel_ptr<Transform> GetParent() { return pParent; }
	void SetParent(noDel_ptr<Transform> obj);
	void SetPosition(float x, float y);
	void SetScale(float x, float y);
	void SetRotation(float x, float y, float z);

private:
	//���[�J���ƃO���[�o���̕ύX���݂��ɔ��f������
	void ConvertLocalAndGlobal();

	//�I�u�W�F�N�g�̍��W�A��]�A�X�P�[���̍X�V
	void UpdateState();

	//�e�I�u�W�F�N�g�ɑ΂��Ďq�I�u�W�F�N�g�̍��W�A��]�A�X�P�[���𑊑ΓI�ɐݒ�
	void SetRelativeState();

	//�l�̗L�������������R���ɂ���
	void RoundingTransform();
};
