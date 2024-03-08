#pragma once

#include <Softbody/SoftBodyForMeshes.h>
#include <PhysicsObject.h>

using namespace Verlet;


class Snake : public SoftBodyForMeshes
{
public:

	Snake(std::vector<PhysicsObject*>& listOfPhysicsObject);
};

