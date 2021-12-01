#pragma once

class Move : public Behaviour {
	~Move() {};
	void Start() override;
	void Update() override;

	void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) override;
};