#include "PhysicsEngine.h"
#include <Graphics/Debugger.h>


bool PhysicsEngine::PhysicsObjectExists(PhysicsObject* physicsObject)
{
	bool exists = false;

	for (PhysicsObject* iteratorObject : physicsObjects)
	{
		if (iteratorObject == physicsObject)
		{
			exists = true;
			return exists;
		}
	}
	return exists;
}

void PhysicsEngine::AddPhysicsObject(PhysicsObject* physicsObject)
{
	if (!PhysicsObjectExists(physicsObject))
	{
		physicsObjects.push_back(physicsObject);
	}
}

void PhysicsEngine::RemovePhysicsObject(PhysicsObject* physicsObject)
{
	if (PhysicsObjectExists(physicsObject))
	{
		physicsObjects.erase(
			std::remove(physicsObjects.begin(), physicsObjects.end(), physicsObject),
			physicsObjects.end());
	}
}

void PhysicsEngine::Update(float deltaTime)
{
	timer += deltaTime;
	
	if (timer >= fixedStepTime)
	{
		UpdatePhysics(deltaTime);

		timer = 0;
	}
}

void PhysicsEngine::UpdatePhysics(float deltaTime)
{
	for (PhysicsObject* iteratorObject : physicsObjects)
	{
		if (iteratorObject->inverse_mass >= 0)
		{
			//Accel change in this frame
			glm::vec3 deltaAcceleration = iteratorObject->acceleration * deltaTime;

			//Rate of velocity change
			iteratorObject->velocity += deltaAcceleration;

			//Rate of velocity change in this frame
			glm::vec3 deltaVelocity = iteratorObject->velocity * deltaTime;

			glm::vec3 predictedPos = iteratorObject->GetPosition() + deltaVelocity;

			for (PhysicsObject* otherObject : physicsObjects)
			{
				if (iteratorObject == otherObject)
					continue;

				if (CollisionAABBvsAABB(iteratorObject->GetModelAABB(), otherObject->GetModelAABB()))
				{
					std::cout << "COLLLLLLIIISSSSION" << std::endl;
				}

			}


			iteratorObject->position = predictedPos;

			iteratorObject->SetPosition(iteratorObject->position);

		}

		/*std::cout << iteratorObject->GetPosition().x << " , "
			<< iteratorObject->GetPosition().y << " , "
			<< iteratorObject->GetPosition().z << std::endl;*/
		
	}
	//Debugger::Print("Physics Update");
}
