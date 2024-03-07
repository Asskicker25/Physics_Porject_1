#include "PhysicsObject.h"
#include <Graphics/Buffer/Triangle.h>
#include <Graphics/Panels/ImguiDrawUtils.h>
#include "PhysicsEngine.h"


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
	return transform.position;
}

glm::vec3 PhysicsObject::GetRotation()
{
	return transform.rotation;
}

void PhysicsObject::SetPosition(const glm::vec3& newPosition)
{
	transform.position = newPosition;
}

void PhysicsObject::SetDrawOrientation(const glm::vec3& newOrientation)
{
	transform.rotation = newOrientation;
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
	isActive = activeSelf;
}

bool PhysicsObject::GetVisible()
{
	return isActive;
}

void PhysicsObject::DrawPhysicsShape()
{
	if (!initialized) return;

	switch (shape)
	{
	case SPHERE:
		Renderer::GetInstance().DrawSphere(((Sphere*)GetTransformedPhysicsShape())->position,
			((Sphere*)GetTransformedPhysicsShape())->radius, shapeColor);

		//Renderer::GetInstance().DrawAABB(GetGraphicsAabb(GetModelAABB()), shapeColor);

		break;
	case PLANE:
		break;
	case TRIANGLE:
		break;
	case AABB:
		Renderer::GetInstance().DrawAABB(GetGraphicsAabb(GetModelAABB()), shapeColor);
		break;
	case CAPSULE:
		break;
	case MESH_OF_TRIANGLES:
		Renderer::GetInstance().DrawAABB(GetGraphicsAabb(GetModelAABB()), shapeColor);
		break;
	default:
		break;
	}
}

void PhysicsObject::DrawPhysicsProperties()
{
	if (!ImGui::TreeNodeEx("PhyProperties", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	ImGuiUtils::DrawFloat("Mass", properties.mass);
	ImGuiUtils::DrawFloat("Bounciness", properties.bounciness);

	if (ImGuiUtils::DrawFloat("Collider Scale", properties.colliderScale))
	{
		//InitializePhysics(shape, mode, collisionMode, isCollisionInvoke);
	}

	float width = 150;
	ImGuiUtils::DrawVector3ImGui("Gravity", properties.gravityScale, 0, width);


	if (ImGuiUtils::DrawVector3ImGui("Offset", properties.offset, 0, width))
	{
		//InitializePhysics(shape, mode, collisionMode, isCollisionInvoke);
	}

	ImGui::TreePop();
}

void PhysicsObject::OnPropertyDraw()
{
	Model::OnPropertyDraw();


	ImGui::Checkbox("###PhyObjEnabled", &isPhysicsEnabled);
	ImGui::SameLine();
	if (!ImGui::TreeNodeEx("Physics Object", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	ImGuiUtils::DrawBool("InvokeCollision", isCollisionInvoke);
	ImGuiUtils::DrawBool("UseBVH", useBvh);
	ImGuiUtils::DrawFloat("BVH_Depth", maxDepth);

	if (ImGuiUtils::DrawDropDown("Mode", modeInt, modeStrings, 2))
	{
		mode = PhysicsMode(modeInt);
	};

	if (ImGuiUtils::DrawDropDown("Shape", shapeInt, shapeStrings, 6))
	{
		shape = (PhysicsShape)shapeInt;
		InitializePhysics(shape, mode, collisionMode, isCollisionInvoke);
	};

	if (ImGuiUtils::DrawDropDown("CollisionMode", collisionModeInt, collModeStrings, 2))
	{
		collisionMode = (CollisionMode)collisionModeInt;
	};

	DrawPhysicsProperties();

	ImGui::TreePop();
}

void PhysicsObject::Render()
{
	Model::Render();

	/*if (Renderer::GetInstance().selectedModel == this)
	{
	}*/
	DrawPhysicsShape();
}

void PhysicsObject::InitializePhysics(PhysicsShape shape, PhysicsMode mode,
	CollisionMode collisionMode, bool isCollisionInvoke)
{
	this->shape = shape;
	this->mode = mode;
	this->collisionMode = collisionMode;
	this->isCollisionInvoke = isCollisionInvoke;

	modeInt = (int)mode;
	shapeInt = (int)shape;
	collisionModeInt = (int)collisionMode;

	initialized = true;

	CalculatePhysicsShape();

	PhysicsEngine::GetInstance().AddPhysicsObject(this);
}

void PhysicsObject::AssignCollisionCallback(const std::function<void(PhysicsObject*)>& collisionCallback)
{
	this->collisionCallback = collisionCallback;
}


const std::function<void(PhysicsObject*)>& PhysicsObject::GetCollisionCallback()
{
	return collisionCallback;
}

Aabb PhysicsObject::CalculateModelAABB()
{
	if (meshes.empty())
	{
		return Aabb{ glm::vec3(0.0f), glm::vec3(0.0f) };
	}

	Aabb minMax;

	minMax.min = meshes[0]->mesh->vertices[0].positions;
	minMax.max = meshes[0]->mesh->vertices[0].positions;

	for (MeshAndMaterial* mesh : meshes)
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
	//glm::mat4 transformMatrix = transform.GetTransformMatrix();

	glm::mat4 transformMatrix = glm::mat4(1.0f);

	transformMatrix = glm::translate(glm::mat4(1.0f), transform.position + properties.offset)
		* glm::toMat4(transform.quaternionRotation)
		* glm::scale(glm::mat4(1.0f), transform.scale * properties.colliderScale);

	//transformMatrix = glm::translate(transformMatrix, glm::vec3(properties.offset));
	//transformMatrix = glm::scale(transformMatrix,glm::vec3( properties.colliderScale));

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
		//position += properties.offset;
		glm::vec3 sideLengths = aabb.max - aabb.min;
		float radius = 0.5f * glm::max(sideLengths.x, glm::max(sideLengths.y, sideLengths.z));
		//radius *= properties.colliderScale;
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
		temp->position = transform.GetTransformMatrix() * glm::vec4(sphere->position, 1.0f);
		temp->position += properties.offset;

		/*temp->radius = sphere->radius * glm::length(model->transform.scale);*/

		temp->radius = sphere->radius *
			glm::max(
				glm::max(transform.scale.x, transform.scale.y),
				transform.scale.z);

		temp->radius *= properties.colliderScale;

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

	for (MeshAndMaterial* mesh : meshes)
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
			if (other->useBvh)
			{
				return CollisionSphereVsMeshOfTriangles(GetModelAABB(),
					dynamic_cast<Sphere*>(GetTransformedPhysicsShape()),
					other->hierarchialAABB->rootNode, other->transform.GetTransformMatrix(),
					other->GetTriangleList(), collisionPoints, collisionNormals, collisionAabbs
				);
			}

			return CollisionSphereVsMeshOfTriangles(dynamic_cast<Sphere*>(GetTransformedPhysicsShape()),
				other->transform.GetTransformMatrix(),
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
					other->hierarchialAABB->rootNode, other->transform.GetTransformMatrix(),
					other->GetTriangleList(), collisionPoints, collisionNormals, collisionAabbs);
			}
			return CollisionAABBVsMeshOfTriangles(GetModelAABB(),
				other->transform.GetTransformMatrix(),
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
					hierarchialAABB->rootNode, transform.GetTransformMatrix(),
					GetTriangleList(), collisionPoints, collisionNormals, collisionAabbs);
			}
			return CollisionAABBVsMeshOfTriangles(other->GetModelAABB(),
				transform.GetTransformMatrix(),
				GetTriangleList(), GetSphereList(),
				collisionPoints, collisionNormals);

		case SPHERE:
			if (other->useBvh)
			{
				return CollisionSphereVsMeshOfTriangles(other->GetModelAABB(),
					dynamic_cast<Sphere*>(other->GetTransformedPhysicsShape()),
					hierarchialAABB->rootNode, transform.GetTransformMatrix(),
					GetTriangleList(), collisionPoints, collisionNormals, collisionAabbs
				);
			}

			return CollisionSphereVsMeshOfTriangles(dynamic_cast<Sphere*>(other->GetTransformedPhysicsShape()),
				transform.GetTransformMatrix(),
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
				transform.GetTransformMatrix(), other->transform.GetTransformMatrix(),
				GetTriangleList(), other->GetTriangleList(), collisionPoints, collisionNormals);
			break;

		}
		break;
#pragma endregion

	}

	return false;
}


