#include "../../framework.h"
#include "../../environment.h"

void Scale::Start() {
}

void Scale::Update() {
	if (Keyboard::Trg(DIK_A)) {
		transform->scale.x += 0.1f;
	}
	if (Keyboard::Trg(DIK_S)) {
		transform->scale.x -= 0.1f;
	}
}

void Scale::OnTrigger2D(noDel_ptr<Collider2D> hitCollider) {
	transform->scale.x *= 2.0f;
	transform->scale.y *= 2.0f;
}