#include "../../framework.h"
#include "../../environment.h"

void Scale::Start() {
	pText = gameObject->FindGameObject("text");
	pText->GetComponent<Font>()->Print(L"?ύX????");
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
}