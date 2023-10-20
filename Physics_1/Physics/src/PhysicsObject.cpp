#include "PhysicsObject.h"
#include <Graphics/Triangle.h>

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

void PhysicsObject::SetVisible(bool activeSelf)
{
	model->isActive = activeSelf;
}

bool PhysicsObject::GetVisible()
{
	return model->isActive;
}

void PhysicsObject::Initialize(Model* model, PhysicsShape shape, PhysicsMode mode)
{
	this->model = model;
	this->shape = shape;
	this->mode = mode;

	CalculatePhysicsShape();
}

Aabb PhysicsObject::CalculateModelAABB()
{
	if (model->meshes.empty())
	{
		return Aabb { glm::vec3(0.0f), glm::vec3(0.0f) };
	}

	Aabb minMax;

	minMax.min = model->meshes[0]->vertices[0].positions;
	minMax.max = model->meshes[0]->vertices[0].positions;

	for (std::shared_ptr<Mesh> mesh : model->meshes)
	{
		Aabb temp = CalculateAABB(mesh->vertices);

		minMax.min.x = std::min(temp.min.x, minMax.min.x);
		minMax.min.y = std::min(temp.min.y, minMax.min.y);
		minMax.min.z = std::min(temp.min.z, minMax.min.z);

		minMax.max.x = std::max(temp.max.x, minMax.max.x);
		minMax.max.y = std::max(temp.max.y, minMax.max.y);
		minMax.max.z = std::max(temp.max.z, minMax.max.z);
	}

	return Aabb{minMax.min, minMax.max };
}

Aabb PhysicsObject::GetModelAABB()
{
	glm::mat4 transformMatrix = model->transform.GetTransformMatrix();

	if (cachedMatrix == transformMatrix)
	{
		return cachedAABB;
	}

	cachedMatrix = transformMatrix;

	Aabb localAABB;
	glm::vec3 minPoint = glm::vec3(transformMatrix * glm::vec4(aabb.min, 1.0f));
	glm::vec3 maxPoint = glm::vec3(transformMatrix * glm::vec4(aabb.max, 1.0f));

	localAABB.min = glm::min(minPoint, maxPoint);
	localAABB.max = glm::max(minPoint, maxPoint);
	
	/*localAABB.min = minPoint;
	localAABB.max = maxPoint;*/

	cachedAABB = localAABB;

	return localAABB;
}

void PhysicsObject::CalculatePhysicsShape()
{
	aabb = CalculateModelAABB();
	Aabb minMax = aabb;

	if (shape == SPHERE)
	{
		glm::vec3 position = (minMax.min + minMax.max) * 0.5f;
		float radius = glm::length(minMax.max - position);
		physicsShape = new Sphere(position, radius);
		transformedPhysicsShape = new Sphere();
	}
	else if (shape == TRIANGLE)
	{
		transformedPhysicsShape = new Triangle();
	}
}

iShape* PhysicsObject::GetTransformedPhysicsShape()
{
	if (shape == SPHERE)
	{
		Sphere* sphere = dynamic_cast<Sphere*>(physicsShape);

		Sphere* temp = dynamic_cast<Sphere*> (transformedPhysicsShape);
		temp->position = model->transform.GetTransformMatrix() * glm::vec4(sphere->position, 1.0f);
		temp->radius = sphere->radius * model->transform.scale.length();

		return transformedPhysicsShape;
	}
	else if (shape == TRIANGLE)
	{
	}

	return transformedPhysicsShape;
}

void PhysicsObject::CalculateTriangleSpheres()
{
	triangles.clear();  

	for (const std::shared_ptr<Mesh>& mesh : model->meshes)
	{
		std::vector<Triangle> meshTriangles;

		meshTriangles.reserve(mesh->triangles.size()); 

		for (const Triangles& triangle : mesh->triangles)
		{
			Triangle temp;

			temp.v1 = triangle.v1;
			temp.v2 = triangle.v2;
			temp.v3 = triangle.v3;

			glm::vec3 sphereCenter = (temp.v1 + temp.v2 + temp.v3) / 3.0f;

			meshTriangles.push_back(std::move(temp));  


		}

		triangles.push_back(std::move(meshTriangles)); 
	}
}

//Aabb PhysicsObject::GetModelAABB()
//{
//
//	glm::mat4 m = model->transform.GetTransformMatrix();
//
//	if (cachedMatrix == m)
//	{
//		return cachedAABB;
//	}
//
//	cachedMatrix = m;
//
//	glm::vec3 t = model->transform.position;
//
//	Aabb b;
//
//	// For all three axes
//	for (int i = 0; i < 3; i++) {
//		// Start by adding in translation
//		b.min[i] = b.max[i] = t[i];
//		// Form extent by summing smaller and larger terms respectively
//		for (int j = 0; j < 3; j++) {
//			float e = m[i][j] * aabb.min[j];
//			float f = m[i][j] * aabb.max[j];
//			if (e < f) {
//				b.min[i] += e;
//				b.max[i] += f;
//			}
//			else {
//				b.min[i] += f;
//				b.max[i] += e;
//			}
//		}
//	}
//
//	cachedAABB = b;
//	return b;
//}


bool PhysicsObject::CheckCollision(PhysicsObject* other, glm::vec3& collisionPoint)
{
	switch (shape)
	{
#pragma region SphereVs
	case SPHERE:
		switch (other->shape)
		{
		case AABB:
			return CollisionSpherevsAABB(dynamic_cast<Sphere*>(GetTransformedPhysicsShape()), other->GetModelAABB());
			break;
		case SPHERE:
			break;
		case TRIANGLE:
			break;
		case PLANE:
			break;
		case CAPSULE:
			break;
		case MESH_OF_TRIANGLES_INDIRECT:
			break;
		case MESH_OF_TRIANGLES_LOCAL_VERTICES:
			break;
		}
	break;
#pragma endregion

#pragma region AABBVs
	case AABB:
		switch (other->shape)
		{
		case AABB:
			return CollisionAABBvsAABB(GetModelAABB(), other->GetModelAABB());
			break;
		case SPHERE:
			break;
		case TRIANGLE:
			break;
		case PLANE:
			break;
		case CAPSULE:
			break;
		case MESH_OF_TRIANGLES_INDIRECT:
			break;
		case MESH_OF_TRIANGLES_LOCAL_VERTICES:
			break;
		}
	break;
#pragma endregion

	}

	return false;
}


