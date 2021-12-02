#include "../../framework.h"
#include "../../environment.h"

void Move::Start() {
	gameObject->AddComponent<Physics2D>(new Physics2D());
	gameObject->AddComponent<Collider2D>(new Collider2D(false));
}


void Move::Update() {
	transform->position.x = (float)Mouse::GetX();
	transform->position.y = (float)Mouse::GetY();
}

void Move::OnTrigger2D(noDel_ptr<Collider2D> hitCollider) {
	transform->rotation.z += 0.1f;
}
void Move::OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) {
	transform->scale.x = 0.5f;
}
void Move::OnTriggerExit2D(noDel_ptr<Collider2D> hitCollider) {
	gameObject->Destroy(hitCollider->gameObject);
}