#pragma once

class Move : public Behaviour {
public:

private:
	void Start() override;
	void Update() override;

	void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) override;
	void OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) override;
	void OnTriggerExit2D(noDel_ptr<Collider2D> hitCollider) override;
};