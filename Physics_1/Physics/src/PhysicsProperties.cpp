#include "PhysicsProperties.h"

const float& PhysicsProperties::GetMass()
{
	return mass;
}

void PhysicsProperties::SetMass(const float& mass)
{
	this->mass = mass;
	inverse_mass = 1.0f / mass;
}
