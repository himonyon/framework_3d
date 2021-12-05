#include "../../../../framework.h"
#include "../../../../environment.h"

Transform::Transform() : Component(eComponentType::Transform) {
	position = { 0,0,0 };
	b_position = position;

	rotation = {0,0,0};
	b_rotation = rotation;
	scale = {1,1,1};
	b_scale = scale;

	localPosition = { 0,0,0 };
	localRotation = { 0,0,0 };
	localScale = { 1,1,1 };

	b_localPosition = localPosition;
	b_localRotation = localRotation;
	b_localScale = localScale;

	transform = noDel_ptr<Transform>(this);
}

Transform::~Transform() {
	//親と子供の親子関係から外す
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

void Transform::SetUpTransform(float x, float y, float z, noDel_ptr<Transform> parent) {
	position = { x,y,z };
	b_position = position;

	rotation = { 0,0,0 };
	b_rotation = rotation;
	scale = { 1,1,1 };
	b_scale = scale;

	if (parent != nullptr) {
		localPosition = position - parent->position;
	}
	else {
		localPosition = {0,0,0};
	}
	localRotation = { 0,0,0 };
	localScale = { 1,1,1 };

	b_localPosition = localPosition;
	b_localRotation = localRotation;
	b_localScale = localScale;
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
		//移動
		child->position = position + (child->localPosition * scale); 
		child->rotation = rotation + child->localRotation;
		child->scale = scale * child->localScale;

		//子オブジェクトの子オブジェクトがあればそのオブジェクトの相対位置を変更
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
		//互いの変更を反映する
		if (diff_position != 0) localPosition += diff_position;
		if (diff_rot != 0) localRotation += diff_rot;
		if (diff_scale != 0) localScale += diff_scale;
		if (diff_localPos != 0) position += diff_localPos;
		if (diff_localRot != 0) rotation += diff_localRot;
		if (diff_localScl != 0) scale += diff_localScl;
	}

	//変更があれば変更状態をOnにする
	if (pChildren.size() != 0) {
		if (diff_position != 0 || diff_rot != 0 || diff_scale != 0 ||
			diff_localPos != 0 || diff_localRot != 0 || diff_localScl != 0) {
			isChanged = true;
		}
	}
}

void Transform::UpdateState() {
	//親の変化に合わせて子要素の各要素も変化させる
	if (pChildren.size() != 0 && isChanged) {
		SetRelativeState();
	}

	//値の有効桁数を小数点3桁にする
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