#pragma once

class PhysicsProperties
{
private:
	float mass = 1.0f;

public:
	const float& GetMass();
	void SetMass(const float& mass);

	float inverse_mass = 1.0f;
	float bounciness = 0.5f;
};

