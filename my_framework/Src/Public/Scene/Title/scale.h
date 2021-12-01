#pragma once

class Scale : public Behaviour {
	~Scale() {};
	void Start() override;
	void Update() override;

	void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) override;
}; 
