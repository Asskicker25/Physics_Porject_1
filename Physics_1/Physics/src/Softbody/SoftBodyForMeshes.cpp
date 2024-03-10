#include <Graphics/Renderer.h>
#include <Graphics/Panels/EditorLayout.h>
#include <Graphics/Panels/ImguiDrawUtils.h>
#include <Graphics/MathUtils.h>
#include "../PhysicsEngine.h"
#include "SoftBodyForMeshes.h"

#define NOMINMAX
#include <Windows.h>

using namespace MathUtilities;

namespace Verlet
{
	
	SoftBodyForMeshes::SoftBodyForMeshes()
	{
		name = "SoftBodyMesh";
		InitializeEntity(this);
		PhysicsEngine::GetInstance().AddSoftBodyObject(this);
	}

	SoftBodyForMeshes::~SoftBodyForMeshes()
	{
		PhysicsEngine::GetInstance().RemoveSoftBodyObject(this);
	}

	void SoftBodyForMeshes::InitializeSoftBody()
	{
		glm::mat4 transformMatrix = transform.GetTransformMatrix();

		for (MeshAndMaterial* mesh : meshes)
		{
			std::vector<PointerToVertex> newListOfVertices;
			std::vector<PointerToIndex> newListOfIndices;

			newListOfVertices.reserve((size_t)mesh->mesh->vertices.size());
			newListOfIndices.reserve((size_t)mesh->mesh->indices.size());

			for (Vertex& vertexInMesh : mesh->mesh->vertices)
			{
				newListOfVertices.push_back({ vertexInMesh, transformMatrix });
			}

			for (unsigned int& indexInMesh : mesh->mesh->indices)
			{
				newListOfIndices.push_back({ indexInMesh });

			}

			mListOfMeshes.push_back({ newListOfVertices, newListOfIndices });
		}

		SetupNodes();
		SetupSticks();
	}

	void SoftBodyForMeshes::UpdateSoftBody(float deltaTime)
	{
		BaseSoftBody::UpdateSoftBody(deltaTime);
	}

	void SoftBodyForMeshes::SetupNodes()
	{
		mListOfNodes.reserve(mListOfMeshes.size());
		glm::mat4 transformMat = transform.GetTransformMatrix();

		unsigned int i = 0;
		for (MeshHolder& mesh : mListOfMeshes)
		{

			Node* node = new Node(mesh.mListOfVertices,
				transformMat, mNodeRadius, IsNodeLocked(i));

			mListOfNodes.push_back(node);
			i++;
		}
	}

	void SoftBodyForMeshes::SetupSticks()
	{
		for (int i = 0; i < mListOfNodes.size() - 1; i++)
		{
			AddStickBetweenNodeIndex(i, i + 1);
		}
	}

	void SoftBodyForMeshes::InitializeLockNodes(std::vector<unsigned int> indexToLock)
	{
		mIndexesToLock = indexToLock;
	}

	void SoftBodyForMeshes::LockNodeAtIndex(int index)
	{
		if (mListOfNodes.size() == 0) return;

		mListOfNodes[index]->mIsLocked = true;
		mListOfLockedNodes.push_back(mListOfNodes[index]);
	}

	bool SoftBodyForMeshes::IsNodeLocked(unsigned int& currentIndex)
	{
		for (unsigned int& index : mIndexesToLock)
		{
			if (index == currentIndex) return true;
		}

		return false;
	}

	void SoftBodyForMeshes::AddStickBetweenNodeIndex(unsigned int nodeAIndex, unsigned int nodeBIndex)
	{
		Node* nodeA = mListOfNodes[nodeAIndex];
		Node* nodeB = mListOfNodes[nodeBIndex];

		mListOfSticks.push_back(new Stick(nodeA, nodeB));
	}


	void SoftBodyForMeshes::UpdateModelVertices()
	{
		EnterCriticalSection(mCriticalSection);

		glm::vec3 pos;

		for (Node* node : mListOfNodes)
		{

			for (PointerToVertex& pointerToVertex : node->mPointerToVertices)
			{
				pos = glm::inverse(transform.GetTransformMatrix()) * glm::vec4(node->mCurrentPosition, 1.0f);
				pos += pointerToVertex.mOffsetFromCenter;
				pointerToVertex.mPointerToVertex->positions = pos;
			}
		}

		LeaveCriticalSection(mCriticalSection);
	}

	void SoftBodyForMeshes::UpdateModelNormals()
	{
		EnterCriticalSection(mCriticalSection);

		for (MeshHolder& mesh : mListOfMeshes)
		{
			for (PointerToVertex& vertex : mesh.mListOfVertices)
			{
				vertex.mPointerToVertex->normals = glm::vec3(0);
			}
		}

		LeaveCriticalSection(mCriticalSection);

		for (MeshAndMaterial* meshAndMat : meshes)
		{
			std::shared_ptr<Mesh>& mesh = meshAndMat->mesh;
			std::vector<Triangles>& listOfTriangles = mesh->triangles;

			int size = listOfTriangles.size();

			for (int i = 0; i < size; i += 3)
			{
				unsigned int vertIndexA = mesh->indices[i];
				unsigned int vertIndexB = mesh->indices[i + 1];
				unsigned int vertIndexC = mesh->indices[i + 2];

				Vertex& vertA = mesh->vertices[vertIndexA];
				Vertex& vertB = mesh->vertices[vertIndexB];
				Vertex& vertC = mesh->vertices[vertIndexC];

				glm::vec3 edgeAB = vertB.positions - vertA.positions;
				glm::vec3 edgeAC = vertC.positions - vertA.positions;

				glm::vec3 normal = glm::normalize(glm::cross(edgeAB, edgeAC));

				CleanZeros(normal);

				vertA.normals += normal;
				vertB.normals += normal;
				vertC.normals += normal;

			}
		}

		EnterCriticalSection(mCriticalSection);

		for (MeshHolder& mesh : mListOfMeshes)
		{
			for (PointerToVertex& vertex : mesh.mListOfVertices)
			{
				vertex.mPointerToVertex->normals = glm::normalize(vertex.mPointerToVertex->normals);

			}
		}

		LeaveCriticalSection(mCriticalSection);
	}

	void SoftBodyForMeshes::ApplyCollision(float deltaTime)
	{

		std::vector<glm::vec3> collisionPts, collisionNr;

		for (Node* node : mListOfNodes)
		{
			node->mIsColliding = false;
		}

		

		for (PhysicsObject* phyObj : mListOfCollidersToCheck)
		{
			int numOfCollisions = 0;

			for (Node* node : mListOfNodes)
			{
				if (node->mIsLocked) continue;

				bool nodeCollided = false;

				Sphere nodeSphere(node->mCurrentPosition, node->mRadius);

				if (collisionMode == TRIGGER) continue;

				collisionPts.clear();
				collisionNr.clear();

				switch (phyObj->shape)
				{
				case SPHERE:

					if (CollisionSphereVSSphere(&nodeSphere, (Sphere*)phyObj->transformedPhysicsShape, collisionPts, collisionNr))
					{
						numOfCollisions++;
						nodeCollided = true;
					}
					break;

				case AABB:

					if (CollisionSpherevsAABB(&nodeSphere, phyObj->GetModelAABB(), true, collisionPts, collisionNr))
					{
						numOfCollisions++;
						nodeCollided = true;
					}

					break;

				}

				if (!nodeCollided) continue;

				EnterCriticalSection(mCriticalSection);

				glm::vec3 normal = glm::vec3(0.0f);
				glm::vec3 collisionPt = glm::vec3(0.0f);

				for (size_t i = 0; i < collisionNr.size(); i++)
				{
					if (glm::length(collisionNr[i]) == 0)
					{
						normal += (collisionNr[i]);
					}
					else
					{
						normal += glm::normalize(collisionNr[i]);
					}
				}

				for (size_t i = 0; i < collisionPts.size(); i++)
				{

					collisionPt += (collisionPts[i]);
				}

				normal = normal / (float)collisionNr.size();
				collisionPt = collisionPt / (float)collisionPts.size();

				glm::vec3 reflected = glm::reflect(glm::normalize(node->velocity), normal);
				node->velocity = reflected * glm::length(node->velocity) * 0.5f;
				node->velocity *= mBounceFactor;
				//node->velocity = glm::vec3(0);

				node->mCurrentPosition = collisionPt + ( reflected * node->mRadius);
				//node->mIsColliding = true;
				//node->mOldPositionm = node->mCurrentPosition;

				LeaveCriticalSection(mCriticalSection);
			}

		}
	}

	void SoftBodyForMeshes::AddForceToRandomNode(glm::vec3 velocity)
	{
		int index = MathUtils::GetRandomIntNumber(0, mListOfNodes.size() - 1);

		mListOfNodes[index]->velocity = velocity;
	}

	void SoftBodyForMeshes::Render()
	{
		BaseSoftBody::Render();
	}

	void SoftBodyForMeshes::OnPropertyDraw()
	{
		BaseSoftBody::OnPropertyDraw();

	}


}
