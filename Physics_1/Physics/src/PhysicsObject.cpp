#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
	float inverse_mass = 1.0f;

	glm::vec3 position = glm::vec3(0.0f);
	//glm::vec3 oldPosition = glm::vec3(0.0f);
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);
}

PhysicsObject::~PhysicsObject()
{
}

glm::vec3 PhysicsObject::GetPosition()
{
	return model->transform.position;
}

glm::vec3 PhysicsObject::GetRotation()
{
	return model->transform.rotation;
}

void PhysicsObject::SetPosition(const glm::vec3& newPosition)
{
	model->transform.position = newPosition;
}

void PhysicsObject::SetDrawOrientation(const glm::vec3& newOrientation)
{
	model->transform.rotation = newOrientation;
}

void PhysicsObject::Initialize(Model* model)
{
	this->model = model;
	aabb = CalculateModelAABB();
	AABB temp = GetModelAABB();
}

AABB PhysicsObject::CalculateModelAABB()
{
	if (model->meshes.empty()) 
	{
		return AABB{ glm::vec3(0.0f), glm::vec3(0.0f) };
	}

	std::vector<AABB> meshAABBs;

	for (std::shared_ptr<Mesh> mesh : model->meshes)
	{
		meshAABBs.push_back(CalculateAABB(mesh->vertices));
	}

	return CombineAABBs(meshAABBs);
}

AABB PhysicsObject::GetModelAABB()
{
	glm::mat4 transformMatrix = model->transform.GetTransformMatrix();

	if (cachedMatrix == transformMatrix)
	{
		return cachedAABB;
	}

	cachedMatrix = transformMatrix;

	AABB localAABB;
	glm::vec3 minPoint = glm::vec3(transformMatrix * glm::vec4(aabb.min, 1.0f));
	glm::vec3 maxPoint = glm::vec3(transformMatrix * glm::vec4(aabb.max, 1.0f));

	localAABB.min = glm::min(minPoint, maxPoint);
	localAABB.max = glm::max(minPoint, maxPoint);

	cachedAABB = localAABB;

	return localAABB;
}


