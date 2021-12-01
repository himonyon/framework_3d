#include "../../../../framework.h"
#include "../../../../environment.h"

Transform::Transform(float x, float y, float z, noDel_ptr<Transform> parent) : Component(eComponentType::Transform) {
	position = { x,y,z };
	b_position = { x,y,z };

	rotation = {0,0,0};
	b_rotation = {0,0,0};
	scale = {1,1,1};
	b_scale = {1,1,1};

	SetParent(parent);
	if (parent == nullptr) {
		localPosition = { 0,0,0 };
		localRotation = { 0,0,0 };
		localScale = { 1,1,1 };
	}
	else {
		localPosition = position - parent->position;
		localRotation = rotation - parent->rotation;;
		localScale = scale / parent->scale;
	}

	b_localPosition = localPosition;
	b_localRotation = localRotation;
	b_localScale = localScale;

	transform = noDel_ptr<Transform>(this);
}
Transform::~Transform() {
	//�e�Ǝq���̐e�q�֌W����O��
	if (pParent != nullptr) {
		for (int i = 0; i < pParent->pChildren.size(); i++) {
			if (pParent->pChildren[i].get() == this) {
				pParent->pChildren.erase(pParent->pChildren.begin() + i);
				break;
			}
		}
	}

	if (pChildren.size() != 0) {
		for (auto& child : pChildren) {
			if (pParent != nullptr) child->SetParent(pParent);
			else child->pParent = nullptr;
		}
	}
}

void Transform::Execute() {
	for (auto& m : messages) {
		if (m == L"ConvertLG") {
			ConvertLocalAndGlobal();
		}
		else if (m == L"UpdateTrans") {
			UpdateState();
		}
	}

	messages.clear();
}

void Transform::SetRelativeState() {

	for (noDel_ptr<Transform> child : pChildren) {
		//�ړ�
		child->position = position + (child->localPosition * scale); 
		child->rotation = rotation + child->localRotation;
		child->scale = scale * child->localScale;

		//�q�I�u�W�F�N�g�̎q�I�u�W�F�N�g������΂��̃I�u�W�F�N�g�̑��Έʒu��ύX
		if (child->pChildren.size() != 0) child->SetRelativeState();

		child->isChanged = false;
		child->b_position = child->position;
		child->b_rotation = child->rotation;
		child->b_scale = child->scale;
	}
}

void Transform::ConvertLocalAndGlobal() {
	//Global
	stVector3 diff_position = position - b_position;
	stVector3 diff_rot = rotation - b_rotation;
	stVector3 diff_scale = scale - b_scale;
	//Local
	stVector3 diff_localPos = localPosition - b_localPosition;
	stVector3 diff_localRot = localRotation - b_localRotation;
	stVector3 diff_localScl = localScale - b_localScale;

	if (pParent != nullptr) {
		//�݂��̕ύX�𔽉f����
		if (diff_position != 0) localPosition += diff_position;
		if (diff_rot != 0) localRotation += diff_rot;
		if (diff_scale != 0) localScale += diff_scale;
		if (diff_localPos != 0) position += diff_localPos;
		if (diff_localRot != 0) rotation += diff_localRot;
		if (diff_localScl != 0) scale += diff_localScl;
	}

	//�ύX������ΕύX��Ԃ�On�ɂ���
	if (pChildren.size() != 0) {
		if (diff_position != 0 || diff_rot != 0 || diff_scale != 0 ||
			diff_localPos != 0 || diff_localRot != 0 || diff_localScl != 0) {
			isChanged = true;
		}
	}
}

void Transform::UpdateState() {
	//�e�̕ω��ɍ��킹�Ďq�v�f�̊e�v�f���ω�������
	if (pChildren.size() != 0 && isChanged) {
		SetRelativeState();
	}

	//�l�̗L�������������_3���ɂ���
	RoundingTransform();

	isChanged = false;
	b_position = position;
	b_rotation = rotation;
	b_scale = scale;
	b_localPosition = localPosition;
	b_localRotation = localRotation;
	b_localScale = localScale;
}


bool Transform::IsParentObjEnable() {
	if (pParent == nullptr) return true;
	if (!pParent->gameObject->IsObjEnable()) return false;
	return pParent->IsParentObjEnable();
}

void Transform::RoundingTransform() {
	position = RoundDec3forVector(position);
	rotation = RoundDec3forVector(rotation);
	scale = RoundDec3forVector(scale);

	localPosition = RoundDec3forVector(localPosition);
	localRotation = RoundDec3forVector(localRotation);
	localScale = RoundDec3forVector(localScale);
}

//setter/getter
void Transform::SetParent(noDel_ptr<Transform> obj) {
	if (obj == nullptr) return;
	pParent = obj;
	obj->pChildren.emplace_back(this);
}
void Transform::SetPosition(float x, float y) {
	position.x = x;
	position.y = y;
}
void Transform::SetScale(float x, float y) {
	scale.x = x;
	scale.y = y;
}
void Transform::SetRotation(float x, float y, float z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}