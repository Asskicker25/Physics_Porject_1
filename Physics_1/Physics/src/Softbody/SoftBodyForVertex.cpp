#include <Graphics/Renderer.h>
#include <Graphics/Panels/EditorLayout.h>
#include <Graphics/Panels/ImguiDrawUtils.h>
#include <Graphics/MathUtils.h>
#include "../PhysicsEngine.h"

#include <Windows.h>

#include "SoftBodyForVertex.h"

using namespace MathUtilities;

namespace Verlet
{
	struct Vec3Comparator {
		bool operator()(const glm::vec3& a, const glm::vec3& b) const {
			// Define your comparison logic here
			// For example, you can compare individual components
			if (a.x != b.x) return a.x < b.x;
			if (a.y != b.y) return a.y < b.y;
			return a.z < b.z;
		}
	};

	SoftBodyForVertex::SoftBodyForVertex()
	{
		name = "SoftBodyVertex";
		InitializeEntity(this);
		PhysicsEngine::GetInstance().AddSoftBodyObject(this);
	}

	SoftBodyForVertex::~SoftBodyForVertex()
	{
		PhysicsEngine::GetInstance().RemoveSoftBodyObject(this);
	}

	void SoftBodyForVertex::InitializeSoftBody()
	{
		mListOfVertices.clear();
		mListOfIndices.clear();
		mListOfNodes.clear();
		mListOfSticks.clear();
		mListOfCollidersToCheck.clear();
		mListOfLockedNodes.clear();

		glm::mat4 transformMatrix = transform.GetTransformMatrix();
		/*glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0))
			* glm::mat4(transform.quaternionRotation)
			* glm::scale(glm::mat4(1.0f), transform.scale);*/

		int i = 0;
		int prevSize = 0;

		for (MeshAndMaterial* mesh : meshes)
		{
			prevSize = mListOfVertices.size();

			for (Vertex& vertexInMesh : mesh->mesh->vertices)
			{
				mListOfVertices.push_back({ vertexInMesh, transformMatrix });
			}
			for (unsigned int& indexInMesh : mesh->mesh->indices)
			{
				mListOfIndices.push_back({ indexInMesh, prevSize });
			}

			i++;
		}

		SetupNodes();
		SetupSticks();

	}

	void SoftBodyForVertex::UpdateSoftBody(float deltaTine, CRITICAL_SECTION& criticalSection)
	{
		BaseSoftBody::UpdateSoftBody(deltaTine,criticalSection);
	}

	void SoftBodyForVertex::SetupNodes()
	{
		mListOfNodes.reserve(mListOfVertices.size());
		glm::mat4 transformMat = transform.GetTransformMatrix();

		for (PointerToVertex& pos : mListOfVertices)
		{
			std::vector<PointerToVertex> posVector;
			posVector.push_back(pos);

			Node* node = new Node(posVector, transformMat, mNodeRadius);


			if (IsNodeLocked(node))
			{
				node->mIsLocked = true;

				mListOfLockedNodes.push_back(node);
			}

			mListOfNodes.push_back(node);
		}
	}

	void SoftBodyForVertex::SetupSticks()
	{

		for (unsigned int i = 0; i < mListOfIndices.size(); i += 3)
		{
			int index2 = i + 1;
			int index3 = i + 2;

			Node* node1 = mListOfNodes[mListOfIndices[i].mLocalIndex];
			Node* node2 = mListOfNodes[mListOfIndices[(index2)].mLocalIndex];
			Node* node3 = mListOfNodes[mListOfIndices[(index3)].mLocalIndex];

			node1->mListOfIndexs.push_back(mListOfIndices[i].mLocalIndex);
			node1->mListOfIndexs.push_back(mListOfIndices[index2].mLocalIndex);
			node1->mListOfIndexs.push_back(mListOfIndices[index3].mLocalIndex);

			node2->mListOfIndexs.push_back(mListOfIndices[i].mLocalIndex);
			node2->mListOfIndexs.push_back(mListOfIndices[index2].mLocalIndex);
			node2->mListOfIndexs.push_back(mListOfIndices[index3].mLocalIndex);

			node3->mListOfIndexs.push_back(mListOfIndices[i].mLocalIndex);
			node3->mListOfIndexs.push_back(mListOfIndices[index2].mLocalIndex);
			node3->mListOfIndexs.push_back(mListOfIndices[index3].mLocalIndex);
			

			mListOfSticks.push_back(new Stick(node1, node2));
			mListOfSticks.push_back(new Stick(node2, node3));
			mListOfSticks.push_back(new Stick(node3, node1));
		}


		for (Node* node : mListOfNodes)
		{
			for (Node* otherNode : mListOfLockedNodes)
			{
				if (node == otherNode) continue;

				float magnitude = glm::dot(otherNode->mCurrentPosition, node->mCurrentPosition);


				if (magnitude * magnitude < mLockAffectDisatance * mLockAffectDisatance)
				{
					mListOfSticks.push_back(new Stick(node, otherNode));
				}
			}
		}



	}

	void SoftBodyForVertex::UpdatModelVertices()
	{
		EnterCriticalSection(mCriticalSection);

		for (Node* node : mListOfNodes)
		{
			node->mPointerToVertices[0].mPointerToVertex->positions =
				glm::inverse(transform.GetTransformMatrix()) * glm::vec4(node->mCurrentPosition, 1.0f);
			//node->mPointerToVertices[0].mPointerToVertex->enabled = node->mEnabled ? 1.0f : 0.0f;
		}

		LeaveCriticalSection(mCriticalSection);

	}

	void SoftBodyForVertex::UpdateModelNormals()
	{
		EnterCriticalSection(mCriticalSection);

		for (PointerToVertex& vertex : mListOfVertices)
		{
			vertex.mPointerToVertex->normals = glm::vec3(0);
		}

		//LeaveCriticalSection(mCriticalSection);

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


		//EnterCriticalSection(mCriticalSection);

		for (PointerToVertex& vertex : mListOfVertices)
		{
			vertex.mPointerToVertex->normals = glm::normalize(vertex.mPointerToVertex->normals);
		}
		LeaveCriticalSection(mCriticalSection);

	}

	bool SoftBodyForVertex::IsNodeLocked(Node* node)
	{
		for (LockNode& lockNode : mListOfLockNodes)
		{
			float length = glm::length(node->mCurrentPosition - lockNode.center);

			if (length > lockNode.radius) continue;

			return true;

		}

		return false;
	}


	void SoftBodyForVertex::Render()
	{
		if (!showDebugModels) return;

		BaseSoftBody::Render();

		for (LockNode& node : mListOfLockNodes)
		{
			Renderer::GetInstance().DrawSphere(node.center, node.radius, lockNodeColor);
		}

	}

	void SoftBodyForVertex::OnPropertyDraw()
	{
		BaseSoftBody::OnPropertyDraw();
	}

	void SoftBodyForVertex::AddLockNode(glm::vec3 posOffset, float radius)
	{
		mListOfLockNodes.push_back({ transform.position + posOffset , radius });
	}

	void SoftBodyForVertex::AddForceToRandomNode(glm::vec3 velocity)
	{
		int index = MathUtils::GetRandomIntNumber(0, mListOfNodes.size() - 1);

		mListOfNodes[index]->velocity = velocity;
	}

	void SoftBodyForVertex::DisconnectRandomStick()
	{
		Stick* stick = mListOfSticks[MathUtils::GetRandomIntNumber(0, mListOfSticks.size())];
		DisconnectStick(stick);
	}

	void SoftBodyForVertex::DisconnectRandomNode()
	{
		Node* node = mListOfNodes[MathUtils::GetRandomIntNumber(0, mListOfNodes.size())];
		
		for (Stick* stick : node->mListOfConnectedSticks)
		{
			DisconnectStick(stick);
		}
	}

}
