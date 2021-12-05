#pragma once

class Move : public Behaviour {
public:

private:
	void Start() override;
	void Update() override;

	void OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) override;
	void OnCollisionEnter2D(noDel_ptr<Collider2D> hitCollider) override;
	void OnCollisionExit2D(noDel_ptr<Collider2D> hitCollider) override;
};