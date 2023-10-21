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

		collisionPoints.clear();

		std::vector<glm::vec3> collisionNormals;

#pragma region CheckingCollision

		for (PhysicsObject* otherObject : physicsObjects)
		{
			if (iteratorObject == otherObject)
				continue;

			if (CollisionAABBvsAABB(iteratorObject->GetModelAABB(), otherObject->GetModelAABB()))
			{
				std::vector<glm::vec3> perObjectCollisions;
				std::vector<glm::vec3> perObjectNormals;

				//std::cout << "AABB TRUE" << std::endl;
				//iteratorObject->SetVisible(false);
				if (HandleCollision(iteratorObject, otherObject, perObjectCollisions, perObjectNormals))
				{
					collisionPoints.insert(collisionPoints.end(), perObjectCollisions.begin(), perObjectCollisions.end());
					collisionNormals.insert(collisionNormals.end(), perObjectNormals.begin(), perObjectNormals.end());
				}
			}

		}
#pragma endregion

#pragma region UpdatingPosition

		iteratorObject->SetCollisionPoints(collisionPoints);

		//Accel change in this frame
		glm::vec3 deltaAcceleration = iteratorObject->acceleration * deltaTime * iteratorObject->inverse_mass;

		if (collisionPoints.size() != 0)
		{

			for (size_t i = 0; i < collisionNormals.size(); i++) 
			{
				glm::vec3 normal = glm::normalize(collisionNormals[i]);

				// Calculate the dot product of the current velocity and the collision normal
				float dotProduct = glm::dot(iteratorObject->velocity, normal);

				// If the dot product is positive, it means the object is moving away from the collision.
				if (dotProduct > 0.0f) 
				{
					// Adjust the velocity to reflect the collision by subtracting the portion along the normal
					iteratorObject->velocity -= dotProduct * normal * iteratorObject->inverse_mass;
				}
			}
			
		}
		else
		{
			//Rate of velocity change
			iteratorObject->velocity += deltaAcceleration;
		}

		//Rate of velocity change in this frame
		glm::vec3 deltaVelocity = iteratorObject->velocity * deltaTime;

		glm::vec3 predictedPos = iteratorObject->GetPosition() + deltaVelocity;

		iteratorObject->position = predictedPos;

		iteratorObject->SetPosition(iteratorObject->position);

#pragma endregion

		/*std::cout << iteratorObject->GetPosition().x << " , "
			<< iteratorObject->GetPosition().y << " , "
			<< iteratorObject->GetPosition().z << std::endl;*/

	}
	//Debugger::Print("Physics Update");
}

bool PhysicsEngine::HandleCollision(PhysicsObject* first, PhysicsObject* second,
	std::vector<glm::vec3>& collisionPoints,
	std::vector<glm::vec3>& collisionNormal)
{
	if (first->CheckCollision(second, collisionPoints, collisionNormal))
	{
		//first->SetVisible(false);
		std::cout << collisionNormal.size() << std::endl;
		return true;
		//std::cout << "COLLLLLLIIISSSSION" << std::endl;
	}

	return false;
}