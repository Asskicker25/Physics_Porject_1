#include "PhysicsObject.h"
#include <Graphics/Triangle.h>

PhysicsObject::PhysicsObject()
{
	glm::vec3 position = glm::vec3(0.0f);
	//glm::vec3 oldPosition = glm::vec3(0.0f);
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);
	isCollisionInvoke = false;
	collisionCallback = nullptr;
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

const std::vector<Triangle>& PhysicsObject::GetTriangleList()
{
	return triangles;
}

const std::vector<Sphere*>& PhysicsObject::GetSphereList()
{
	return triangleSpheres;
}

const std::vector<glm::vec3>& PhysicsObject::GetCollisionPoints()
{
	return collisionPoints;
}

const std::vector<glm::vec3>& PhysicsObject::GetCollisionNormals()
{
	return collisionNormals;
}

const std::vector<Aabb>& PhysicsObject::GetCollisionAabbs()
{
	return collisionAabbs;
}

void PhysicsObject::SetCollisionPoints(const std::vector<glm::vec3>& collisionPoints)
{
	this->collisionPoints = collisionPoints;
}

void PhysicsObject::SetCollisionNormals(const std::vector<glm::vec3>& collisionNormals)
{
	this->collisionNormals = collisionNormals;
}

void PhysicsObject::SetCollisionAabbs(const std::vector<Aabb>& collisionAabs)
{
	this->collisionNormals = collisionNormals;
}

void PhysicsObject::SetVisible(bool activeSelf)
{
	model->isActive = activeSelf;
}

bool PhysicsObject::GetVisible()
{
	return model->isActive;
}

void PhysicsObject::Initialize(Model* model, PhysicsShape shape, PhysicsMode mode,
	CollisionMode collisionMode, bool isCollisionInvoke)
{
	this->model = model;
	this->shape = shape;
	this->mode = mode;
	this->collisionMode = collisionMode;
	this->isCollisionInvoke = isCollisionInvoke;

	CalculatePhysicsShape();

}

void PhysicsObject::AssignCollisionCallback(const std::function<void(PhysicsObject*)>& collisionCallback)
{
	this->collisionCallback = collisionCallback;
}

Model* PhysicsObject::GetModel()
{
	return model;
}

const std::function<void(PhysicsObject*)>& PhysicsObject::GetCollisionCallback()
{
	return collisionCallback;
}

Aabb PhysicsObject::CalculateModelAABB()
{
	if (model->meshes.empty())
	{
		return Aabb{ glm::vec3(0.0f), glm::vec3(0.0f) };
	}

	Aabb minMax;

	minMax.min = model->meshes[0]->mesh->vertices[0].positions;
	minMax.max = model->meshes[0]->mesh->vertices[0].positions;

	for (MeshAndMaterial* mesh : model->meshes)
	{
		Aabb temp = CalculateAABB(mesh->mesh->vertices);

		minMax.min.x = std::min(temp.min.x, minMax.min.x);
		minMax.min.y = std::min(temp.min.y, minMax.min.y);
		minMax.min.z = std::min(temp.min.z, minMax.min.z);

		minMax.max.x = std::max(temp.max.x, minMax.max.x);
		minMax.max.y = std::max(temp.max.y, minMax.max.y);
		minMax.max.z = std::max(temp.max.z, minMax.max.z);
	}

	return Aabb{ minMax.min, minMax.max };
}

Aabb PhysicsObject::GetModelAABB()
{
	glm::mat4 transformMatrix = model->transform.GetTransformMatrix();

	if (cachedMatrix == transformMatrix)
	{
		return cachedAABB;
	}

	cachedMatrix = transformMatrix;

	glm::vec3 originalMinV = aabb.min;
	glm::vec3 originalMaxV = aabb.max;

	glm::vec4 vertices[8];
	vertices[0] = transformMatrix * glm::vec4(originalMinV.x, originalMinV.y, originalMinV.z, 1.0f);
	vertices[1] = transformMatrix * glm::vec4(originalMinV.x, originalMinV.y, originalMaxV.z, 1.0f);
	vertices[2] = transformMatrix * glm::vec4(originalMinV.x, originalMaxV.y, originalMinV.z, 1.0f);
	vertices[3] = transformMatrix * glm::vec4(originalMinV.x, originalMaxV.y, originalMaxV.z, 1.0f);
	vertices[4] = transformMatrix * glm::vec4(originalMaxV.x, originalMinV.y, originalMinV.z, 1.0f);
	vertices[5] = transformMatrix * glm::vec4(originalMaxV.x, originalMinV.y, originalMaxV.z, 1.0f);
	vertices[6] = transformMatrix * glm::vec4(originalMaxV.x, originalMaxV.y, originalMinV.z, 1.0f);
	vertices[7] = transformMatrix * glm::vec4(originalMaxV.x, originalMaxV.y, originalMaxV.z, 1.0f);

	glm::vec3 transformedMinV = glm::vec3(vertices[0]);
	glm::vec3 transformedMaxV = glm::vec3(vertices[0]);

	for (int i = 1; i < 8; ++i) {
		transformedMinV = glm::min(transformedMinV, glm::vec3(vertices[i]));
		transformedMaxV = glm::max(transformedMaxV, glm::vec3(vertices[i]));
	}

	Aabb localAabb(transformedMinV, transformedMaxV);

	cachedAABB = localAabb;

	return localAabb;
}

Aabb PhysicsObject::GetAABB()
{
	return aabb;
}

void PhysicsObject::AddExludingPhyObj(PhysicsObject* phyObj)
{
	listOfExcludingPhyObjects.push_back(phyObj);
}

bool PhysicsObject::CheckIfExcluding(PhysicsObject* phyObj)
{
	for (PhysicsObject* phy : listOfExcludingPhyObjects)
	{
		if (phy == phyObj)
		{
			return true;
		}
	}
	return false;
}

void PhysicsObject::CalculatePhysicsShape()
{
	aabb = CalculateModelAABB();


	if (shape == SPHERE)
	{
		glm::vec3 position = (aabb.min + aabb.max) * 0.5f;
		position += properties.offset;
		glm::vec3 sideLengths = aabb.max - aabb.min;
		float radius = 0.5f * glm::max(sideLengths.x, glm::max(sideLengths.y, sideLengths.z));
		radius *= properties.colliderScale;
		physicsShape = new Sphere(position, radius);
		transformedPhysicsShape = new Sphere();
	}
	else if (shape == MESH_OF_TRIANGLES)
	{
		CalculateTriangleSpheres();
		transformedPhysicsShape = new Triangle();
		hierarchialAABB = new HierarchicalAABB(this, maxDepth);
	}
}

iShape* PhysicsObject::GetTransformedPhysicsShape()
{
	if (shape == SPHERE)
	{
		Sphere* sphere = dynamic_cast<Sphere*>(physicsShape);

		Sphere* temp = dynamic_cast<Sphere*> (transformedPhysicsShape);
		temp->position = model->transform.GetTransformMatrix() * glm::vec4(sphere->position, 1.0f);

		/*temp->radius = sphere->radius * glm::length(model->transform.scale);*/

		temp->radius = sphere->radius *
			glm::max(
				glm::max(model->transform.scale.x, model->transform.scale.y),
				model->transform.scale.z);

		return transformedPhysicsShape;
	}
	else if (shape == TRIANGLE)
	{
	}

	return transformedPhysicsShape;
}

void PhysicsObject::CalculateTriangleSpheres()
{
	for (Sphere* sphere : triangleSpheres)
	{
		delete sphere;
	}

	triangles.clear();
	triangleSpheres.clear();

	for (MeshAndMaterial* mesh : model->meshes)
	{
		for (const Triangles& triangle : mesh->mesh->triangles)
		{
			Triangle temp;

			temp.v1 = triangle.v1;
			temp.v2 = triangle.v2;
			temp.v3 = triangle.v3;
			temp.normal = triangle.normal;

			glm::vec3 sphereCenter = (temp.v1 + temp.v2 + temp.v3) / 3.0f;
			float radius = glm::max(glm::distance(sphereCenter, temp.v1),
				glm::max(glm::distance(sphereCenter, temp.v2), glm::distance(sphereCenter, temp.v3)));

			triangles.push_back(std::move(temp));
			triangleSpheres.push_back(new Sphere(sphereCenter, radius));
		}
	}
}

bool PhysicsObject::CheckCollision(PhysicsObject* other,
	std::vector<glm::vec3>& collisionPoints,
	std::vector<glm::vec3>& collisionNormals)
{
	switch (shape)
	{
#pragma region SphereVs
	case SPHERE:
		switch (other->shape)
		{
		case AABB:
			return CollisionSpherevsAABB(dynamic_cast<Sphere*>(GetTransformedPhysicsShape()), other->GetModelAABB(),
				true, collisionPoints, collisionNormals);
		case SPHERE:
			return CollisionSphereVSSphere(dynamic_cast<Sphere*>(GetTransformedPhysicsShape()),
				dynamic_cast<Sphere*>(other->GetTransformedPhysicsShape()),
				collisionPoints, collisionNormals);
		case TRIANGLE:
			break;
		case PLANE:
			break;
		case CAPSULE:
			break;
		case MESH_OF_TRIANGLES:
			if(other->useBvh)
			{
				return CollisionSphereVsMeshOfTriangles(GetModelAABB(),
					dynamic_cast<Sphere*>(GetTransformedPhysicsShape()),
						other->hierarchialAABB->rootNode, other->model->transform.GetTransformMatrix(),
						other->GetTriangleList(), collisionPoints, collisionNormals,collisionAabbs
						);
			}

			return CollisionSphereVsMeshOfTriangles(dynamic_cast<Sphere*>(GetTransformedPhysicsShape()),
				other->model->transform.GetTransformMatrix(),
				other->GetTriangleList(), other->GetSphereList(),
				collisionPoints, collisionNormals);
		}
		break;
#pragma endregion

#pragma region AABBVs
	case AABB:
		switch (other->shape)
		{
		case AABB:
			return CollisionAABBvsAABB(GetModelAABB(), other->GetModelAABB(), collisionPoints, collisionNormals);
		case SPHERE:
			return CollisionSpherevsAABB(dynamic_cast<Sphere*>(other->GetTransformedPhysicsShape()), GetModelAABB(),
				false, collisionPoints, collisionNormals);
		case TRIANGLE:
			break;
		case PLANE:
			break;
		case CAPSULE:
			break;
		case MESH_OF_TRIANGLES:
			if (other->useBvh)
			{
				return CollisionAABBVsMeshOfTriangles(GetModelAABB(),
					other->hierarchialAABB->rootNode, other->model->transform.GetTransformMatrix(),
					other->GetTriangleList(), collisionPoints, collisionNormals, collisionAabbs);
			}
			return CollisionAABBVsMeshOfTriangles(GetModelAABB(),
				other->model->transform.GetTransformMatrix(),
				other->GetTriangleList(), other->GetSphereList(),
				collisionPoints, collisionNormals);
		}
		break;
#pragma endregion

#pragma region MESH_OF_TRIANGLES
	case MESH_OF_TRIANGLES:
		switch (other->shape)
		{
		case AABB:
			if (other->useBvh)
			{
				return CollisionAABBVsMeshOfTriangles(other->GetModelAABB(),
					hierarchialAABB->rootNode, model->transform.GetTransformMatrix(),
					GetTriangleList(), collisionPoints, collisionNormals, collisionAabbs);
			}
			return CollisionAABBVsMeshOfTriangles(other->GetModelAABB(),
				model->transform.GetTransformMatrix(),
				GetTriangleList(), GetSphereList(),
				collisionPoints, collisionNormals);

		case SPHERE:
			if (other->useBvh)
			{
				return CollisionSphereVsMeshOfTriangles(other->GetModelAABB(),
					dynamic_cast<Sphere*>(other->GetTransformedPhysicsShape()),
					hierarchialAABB->rootNode, model->transform.GetTransformMatrix(),
					GetTriangleList(), collisionPoints, collisionNormals, collisionAabbs
				);
			}

			return CollisionSphereVsMeshOfTriangles(dynamic_cast<Sphere*>(other->GetTransformedPhysicsShape()),
				model->transform.GetTransformMatrix(),
				GetTriangleList(), GetSphereList(),
				collisionPoints, collisionNormals);
		case TRIANGLE:
			break;
		case PLANE:
			break;
		case CAPSULE:
			break;
		case MESH_OF_TRIANGLES:

			return CollisionMeshVsMesh(hierarchialAABB->rootNode, other->hierarchialAABB->rootNode,
				model->transform.GetTransformMatrix(), other->model->transform.GetTransformMatrix(),
				GetTriangleList(), other->GetTriangleList(), collisionPoints, collisionNormals);
			break;
			
		}
		break;
#pragma endregion

	}

	return false;
}


