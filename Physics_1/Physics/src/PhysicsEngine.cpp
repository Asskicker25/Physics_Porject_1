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
		if (iteratorObject->mode == PhysicsMode::STATIC)
			continue;

		if (iteratorObject->inverse_mass < 0)
			continue;

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

				collisionPoints.clear();

				if (CollisionAABBvsAABB(iteratorObject->GetModelAABB(), otherObject->GetModelAABB()))
				{
					std::vector<glm::vec3> perObjectCollisions;

					//std::cout << "AABB TRUE" << std::endl;
					//iteratorObject->SetVisible(false);
					if (HandleCollision(iteratorObject, otherObject, perObjectCollisions))
					{
						collisionPoints.insert(collisionPoints.end(), perObjectCollisions.begin(), perObjectCollisions.end());
					}
				}

			}

			iteratorObject->position = predictedPos;

			iteratorObject->SetPosition(iteratorObject->position);


		/*std::cout << iteratorObject->GetPosition().x << " , "
			<< iteratorObject->GetPosition().y << " , "
			<< iteratorObject->GetPosition().z << std::endl;*/
		
	}
	//Debugger::Print("Physics Update");
}

bool PhysicsEngine::HandleCollision(PhysicsObject* first, PhysicsObject* second, std::vector<glm::vec3>& collisionPoints)
{
	if (first->CheckCollision(second, collisionPoints))
	{
		first->SetVisible(false);
		std::cout << collisionPoints.size() << std::endl;
		//std::cout << "COLLLLLLIIISSSSION" << std::endl;
	}
	else
	{
		//std::cout << "Sphere VS AABB False" << std::endl;
	}
	return false;
}