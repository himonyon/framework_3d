#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

Renderer::Renderer(eComponentType type)
	: Component(type)
{
	color = { 1,1,1,1 };
}

Renderer::~Renderer() {
}

stVector3 Renderer::GetPosOnCam() {
	//現在の座標を頂点座標にセット
	stVector3 pos;
	pos.x = transform->position.x;
	pos.y = transform->position.y;
	pos.z = transform->position.z;
	//カメラ座標を加えてスクリーン座標を設定する
	if (Camera::main != nullptr) {
		pos.x -= Camera::main->transform->position.x;
		pos.y -= Camera::main->transform->position.y;
		pos.z -= Camera::main->transform->position.z;
	}
	return stVector3{ pos.x, pos.y, pos.z };
}
stVector3 Renderer::GetRotOnCam() {
	stVector3 rot;
	rot.x = transform->rotation.x;
	rot.y = transform->rotation.y;
	rot.z = transform->rotation.z;

	if (Camera::main != nullptr) {
		rot.x -= Camera::main->transform->rotation.x;
		rot.y -= Camera::main->transform->rotation.y;
		rot.z -= Camera::main->transform->rotation.z;
	}
	return stVector3{ rot.x, rot.y, rot.z };
}
stVector3 Renderer::GetScaleOnCam() {
	stVector3 scl;
	scl.x = transform->scale.x;
	scl.y = transform->scale.y;
	scl.z = transform->scale.z;

	if (Camera::main != nullptr) {
		scl.x *= Camera::main->transform->scale.x;
		scl.y *= Camera::main->transform->scale.y;
		scl.z *= Camera::main->transform->scale.z;
	}

	return stVector3{ scl.x, scl.y, scl.z };
}