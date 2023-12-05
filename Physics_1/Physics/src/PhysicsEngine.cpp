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
		UpdatePhysics(fixedStepTime);

		timer = 0;
	}
}

void PhysicsEngine::SetDebugSpheres(Model* model, int count)
{
	debugSpheres.clear();
	for (int i = 0; i < count; i++)
	{
		debugSpheres.push_back(&model[i]);
	}
}

void PhysicsEngine::UpdatePhysics(float deltaTime)
{
	for (PhysicsObject* iteratorObject : physicsObjects)
	{

		if (iteratorObject->mode == PhysicsMode::STATIC)
			continue;

		if (iteratorObject->properties.inverse_mass < 0)
			continue;

		collisionPoints.clear();

		std::vector<glm::vec3> collisionNormals;

		glm::vec3 iteratorGravity =
			glm::vec3(gravity.x * iteratorObject->properties.gravityScale.x,
				gravity.y * iteratorObject->properties.gravityScale.y,
				gravity.z * iteratorObject->properties.gravityScale.z);


		glm::vec3 deltaAcceleration = iteratorGravity * deltaTime * iteratorObject->properties.inverse_mass;

		iteratorObject->velocity += deltaAcceleration;

		glm::vec3 deltaVelocity = iteratorObject->velocity * deltaTime;

		glm::vec3 predictedPos = iteratorObject->GetPosition() + deltaVelocity;

		iteratorObject->position = predictedPos;

		iteratorObject->SetPosition(iteratorObject->position);

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

					iteratorObject->SetCollisionPoints(collisionPoints);
					iteratorObject->SetCollisionNormals(collisionNormals);
				}

#pragma region CollisionInvoke
				if (collisionPoints.size() > 0)
				{
					if (iteratorObject->isCollisionInvoke)
					{
						if (iteratorObject->GetCollisionCallback() != nullptr)
						{
							iteratorObject->GetCollisionCallback()(otherObject);
						}
					}
				}
#pragma endregion

			}
		}
#pragma endregion

#pragma region UpdatingPosition



		if (iteratorObject->collisionMode == TRIGGER) continue;

		if (collisionPoints.size() != 0)
		{

			glm::vec3 normal = glm::vec3(0.0f);
			glm::vec3 collisionPt = glm::vec3(0.0f);

			for (size_t i = 0; i < collisionNormals.size(); i++)
			{
				normal += glm::normalize(collisionNormals[i]);
			}

			for (size_t i = 0; i < collisionPoints.size(); i++)
			{
				collisionPt += glm::normalize(collisionPoints[i]);
			}

			normal = normal / (float)collisionNormals.size();
			collisionPt = collisionPt / (float)collisionPoints.size();

			if (iteratorObject->mode == KINEMATIC)
			{
				float length = glm::length(iteratorObject->position - collisionPt);
				iteratorObject->position = collisionPt + (normal * length);
				//iteratorObject->velocity = glm::vec3(0);
			}
			else
			{
				glm::vec3 reflected = glm::reflect(glm::normalize(iteratorObject->velocity), normal);
				iteratorObject->velocity = reflected * (/*iteratorObject->properties.bounciness **/ glm::length(iteratorObject->velocity));
			}

			//glm::vec3 reflected = glm::reflect(glm::normalize(iteratorObject->velocity), normal);

			//iteratorObject->velocity = reflected * (/*iteratorObject->properties.bounciness **/ glm::length(iteratorObject->velocity));
		}

#pragma endregion
	}
}

bool PhysicsEngine::HandleCollision(PhysicsObject* first, PhysicsObject* second,
	std::vector<glm::vec3>& collisionPoints,
	std::vector<glm::vec3>& collisionNormal)
{
	if (first->CheckCollision(second, collisionPoints, collisionNormal))
	{
		//first->SetVisible(false);
		//Debugger::Print("Collision Point Count : ",(int) collisionNormal.size());
		return true;
		//std::cout << "COLLLLLLIIISSSSION" << std::endl;
	}

	return false;
}

