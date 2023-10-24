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

		glm::vec3 deltaAcceleration = gravity * deltaTime * iteratorObject->properties.inverse_mass;

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
				}
			}

		}
#pragma endregion

#pragma region UpdatingPosition

		iteratorObject->SetCollisionPoints(collisionPoints);

		//Accel change in this frame

		if (collisionPoints.size() != 0)
		{
			glm::vec3 normal = glm::vec3(0.0f);

			for (size_t i = 0; i < collisionNormals.size(); i++)
			{
				normal += glm::normalize(collisionNormals[i]);
			}

			normal = normal /(float) collisionNormals.size();

			glm::vec3 reflected = glm::reflect(glm::normalize(iteratorObject->velocity), normal);

			iteratorObject->velocity = reflected * (/*iteratorObject->properties.bounciness **/ glm::length(iteratorObject->velocity));
			//Debugger::Print("Velocity", iteratorObject->velocity);
		}

		//iteratorObject->position = iteratorObject->oldPosition;

		//iteratorObject->SetPosition(iteratorObject->position);

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
		//Debugger::Print("Collision Point Count : ",(int) collisionNormal.size());
		return true;
		//std::cout << "COLLLLLLIIISSSSION" << std::endl;
	}

	return false;
}