#include "../../framework.h"
#include "../../../environment.h"

GameObject::GameObject(float x, float y,float z, bool isRender, noDel_ptr<GameObject> parent) {
	//現在の位置と前フレーム一の初期化
	position.x = x;
	position.y = y;
	position.z = z;
	before_position.x = x;
	before_position.y = y;
	before_position.z = z;

	scale.x = 1;
	scale.y = 1;
	scale.z = 1;
	before_scale.x = 1;
	before_scale.y = 1;
	before_scale.z = 1;

	rot.x = 0;
	rot.y = 0;
	rot.z = 0;
	before_rot.x = 0;
	before_rot.y = 0;
	before_rot.z = 0;

	if (parent != nullptr) SetParent(parent);
	executeEnable = true;
	renderEnable = isRender;
}

GameObject::~GameObject() {
}


void GameObject::Execute() {
	stVector3 diff_position = position - before_position;
	stVector3 diff_rot = rot - before_rot;
	stVector3 diff_scale = scale - before_scale;

	if (diff_position != 0 || diff_rot != 0 || diff_scale != 0) {
		for (noDel_ptr<GameObject> child : pChildren) {
			if (diff_position != 0) child->position += diff_position;
			if (diff_rot != 0) child->rot += diff_rot;
			if (diff_scale != 0) {
				child->scale += diff_scale;
			}
		}
	}
	
	before_position = position;
	before_rot = rot;
	before_scale = scale;
}

void GameObject::Render() {
}

//setter/getter
void GameObject::SetParent(noDel_ptr<GameObject> obj) {
	pParent = obj;
	obj->pChildren.emplace_back(this);
}
void GameObject::SetPosition(float x, float y) {
	position.x = x;
	position.y = y;
}
void GameObject::SetScale(float x, float y) {
	scale.x = x;
	scale.y = y;
}
void GameObject::SetRotation(float x, float y, float z) {
	rot.x = x;
	rot.y = y;
	rot.z = z;
}

void GameObject::SetExecuteEnable(bool flag) {
	executeEnable = flag;
}

void GameObject::SetRenderEnable(bool flag) {
	renderEnable = flag;
}

bool GameObject::isExecuteEnable() {
	return executeEnable;
}

bool GameObject::isRenderEnable() {
	return renderEnable;
}