#pragma once

class Scale : public Behaviour {
private:
	noDel_ptr<GameObject> pText = 0;

public:
	~Scale() {};
	void Start() override;
	void Update() override;

	void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) override;
}; 
