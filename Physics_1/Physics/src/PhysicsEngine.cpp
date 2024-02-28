#include "PhysicsEngine.h"
#include <Graphics/Debugger.h>
#include "PhysicsShapeAndCollision.h"


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


PhysicsEngine& PhysicsEngine::GetInstance()
{
	static PhysicsEngine instance;
	return instance;
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

void PhysicsEngine::AddSoftBodyObject(BaseSoftBody* softBody)
{
	listOfSoftBodies.push_back(softBody);
}

void PhysicsEngine::RemoveSoftBodyObject(BaseSoftBody* softBody)
{
	listOfSoftBodies.erase(std::remove(listOfSoftBodies.begin(), listOfSoftBodies.end(), softBody), listOfSoftBodies.end());
}

void PhysicsEngine::Update(float deltaTime)
{
	timer += deltaTime;

	UpdateSoftBodyBufferData();

	if (timer >= fixedStepTime)
	{
		UpdatePhysics(fixedStepTime);
		UpdateSoftBodyPhysics(fixedStepTime);

		timer = 0;
	}
}

void PhysicsEngine::UpdateSoftBodies(float deltaTime, CRITICAL_SECTION& criticalSection)
{

	for (BaseSoftBody* softBody : listOfSoftBodies)
	{
		softBody->UpdateSoftBody(deltaTime, criticalSection);
	}
}

void PhysicsEngine::UpdateSoftBodyBufferData()
{
	for (BaseSoftBody* softBody : listOfSoftBodies)
	{
		softBody->UpdateBufferData();
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
		if (iteratorObject->isEnabled == false)
			continue;

		if (iteratorObject->mode == PhysicsMode::STATIC)
			continue;

		if (iteratorObject->properties.GetInverseMass() < 0)
			continue;

		collisionPoints.clear();

		std::vector<glm::vec3> collisionNormals;

		glm::vec3 iteratorGravity =
			glm::vec3(gravity.x * iteratorObject->properties.gravityScale.x,
				gravity.y * iteratorObject->properties.gravityScale.y,
				gravity.z * iteratorObject->properties.gravityScale.z);


		glm::vec3 deltaAcceleration = iteratorGravity * deltaTime * iteratorObject->properties.GetInverseMass();

		iteratorObject->velocity += deltaAcceleration;

		glm::vec3 deltaVelocity = iteratorObject->velocity * deltaTime;

		glm::vec3 predictedPos = iteratorObject->GetPosition() + deltaVelocity;

		iteratorObject->position = predictedPos;

		iteratorObject->SetPosition(iteratorObject->position);

#pragma region CheckingCollision

		for (PhysicsObject* otherObject : physicsObjects)
		{
			if (otherObject->isEnabled == false)
				continue;

			if (iteratorObject == otherObject)
				continue;

			if (iteratorObject->CheckIfExcluding(otherObject))
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
		return true;
	}

	return false;
}



void PhysicsEngine::UpdateSoftBodyPhysics(float deltaTime)
{
	for (BaseSoftBody* softBody : listOfSoftBodies)
	{
		for (PhysicsObject* phyObj : softBody->mListOfCollidersToCheck)
		{
			std::vector<glm::vec3> collisionPts, collisionNormals;

			if (HandleSoftBodyCollision(softBody, phyObj, collisionPts, collisionNormals))
			{
				Debugger::Print("Colliding");
			}

		}
	}
}


bool PhysicsEngine::HandleSoftBodyCollision(BaseSoftBody* softBody, PhysicsObject* phyObj,
	std::vector<glm::vec3> collisionPt, std::vector<glm::vec3> collisionNormals)
{
	//glm::mat4 transformMat = softBody->transform.GetTransformMatrix();

	int numOfCollisions = 0;

	for (BaseSoftBody::Node* node : softBody->mListOfNodes)
	{
		Sphere nodeSphere(node->mCurrentPosition, node->mRadius);

		if (CollisionSphereVSSphere(&nodeSphere, (Sphere*)phyObj->transformedPhysicsShape, collisionPt, collisionNormals))
		{
			numOfCollisions++;

			if (softBody->collisionMode == TRIGGER) continue;

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

			glm::vec3 reflected = glm::reflect(glm::normalize(node->velocity), normal);
			node->velocity = reflected * (/*iteratorObject->properties.bounciness **/ glm::length(node->velocity)) ;
			node->velocity *= softBody->mBounceFactor;
		}

		
	}

	return numOfCollisions != 0;
}
