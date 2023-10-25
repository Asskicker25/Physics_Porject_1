#pragma once

#include <glm/glm.hpp>

class PhysicsProperties
{
private:
	float mass = 1.0f;

public:
	const float& GetMass();
	void SetMass(const float& mass);

	float inverse_mass = 1.0f;
	float bounciness = 0.5f;

	glm::vec3 gravityScale = glm::vec3(1.0f);
};

