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

	struct SoftBodyForVertex::Node
	{
		Node(PointerToVertex& vertex, bool isLocked = false)
		{
			mIsLocked = isLocked;

			mCurrentPosition = vertex.mLocalVertex.positions;
			mOldPositionm = mCurrentPosition;
			mPointerToVertex = vertex.mPointerToVertex;
		};

		bool mIsLocked = false;

		glm::vec3 mCurrentPosition = glm::vec3(0);
		glm::vec3 mOldPositionm = glm::vec3(0);

		glm::vec3 velocity = glm::vec3(0);

		Vertex* mPointerToVertex = nullptr;
	};

	struct  SoftBodyForVertex::Stick
	{
		Stick(Node* nodeA, Node* nodeB)
		{
			mNodeA = nodeA;
			mNodeB = nodeB;

			mRestLength = glm::distance(nodeA->mCurrentPosition, nodeB->mCurrentPosition);
		};

		bool isConnected = true;
		float mRestLength = 0;
		unsigned int mNumOfIterations = 1;

		Node* mNodeA = nullptr;
		Node* mNodeB = nullptr;
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
		mCriticalSection = &criticalSection;

		UpdateNodePosition(deltaTine);
		SatisfyConstraints(deltaTine);
		UpdateModelData(deltaTine);
	}

	void SoftBodyForVertex::SetupNodes()
	{
		mListOfNodes.reserve(mListOfVertices.size());

		for (PointerToVertex& pos : mListOfVertices)
		{
			Node* node = new Node(pos);


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

	void SoftBodyForVertex::UpdateNodePosition(float deltaTime)
	{
		for (Node* node : mListOfNodes)
		{
			if (node->mIsLocked) continue;

			node->velocity += mGravity * deltaTime;
			//node->mCurrentPosition += node->velocity * deltaTime;

			UpdatePositionByVerlet(node, deltaTime);
		}
	}

	void SoftBodyForVertex::UpdatePositionByVerlet(Node* node, float deltaTime)
	{
		glm::vec3 posBeforUpdate = node->mCurrentPosition;

		node->mCurrentPosition += (posBeforUpdate - node->mOldPositionm) + (node->velocity * (deltaTime * deltaTime));
		node->mOldPositionm = posBeforUpdate;

		CleanZeros(node->mCurrentPosition);
		CleanZeros(node->mOldPositionm);
	}

	void SoftBodyForVertex::SatisfyConstraints(float deltaTime)
	{

		for (unsigned int i = 0; i < mNumOfIterations; i++)
		{
			for (Stick* stick : mListOfSticks)
			{
				if (!stick->isConnected) continue;

				Node* nodeA = stick->mNodeA;
				Node* nodeB = stick->mNodeB;

				glm::vec3 delta = nodeB->mCurrentPosition - nodeA->mCurrentPosition;
				float length = glm::length(delta);

				float diff = (length - stick->mRestLength) / length;

				if (!nodeA->mIsLocked)
				{
					nodeA->mCurrentPosition += delta * 0.5f * diff * mTightness;
				}

				if (!nodeB->mIsLocked)
				{
					nodeB->mCurrentPosition -= delta * 0.5f * diff * mTightness;
				}

				/*glm::vec3 stickCenter = (nodeA->mCurrentPosition + nodeB->mCurrentPosition) / 2.0f;
				glm::vec3 stickDir = glm::normalize((nodeA->mCurrentPosition - nodeB->mCurrentPosition));

				if (!nodeA->mIsLocked)
				{
					nodeA->mCurrentPosition = stickCenter + stickDir * stick->mRestLength * 0.5f;
				}

				if (!nodeB->mIsLocked)
				{
					nodeB->mCurrentPosition = stickCenter - stickDir * stick->mRestLength * 0.5f;
				}*/

				CleanZeros(nodeA->mCurrentPosition);
				CleanZeros(nodeB->mCurrentPosition);
			}
		}
	}


	void SoftBodyForVertex::UpdateModelData(float deltaTime)
	{
		UpdatModelVertices();
		UpdateModelNormals();
	}
	
	void SoftBodyForVertex::UpdateBufferData()
	{
		for (MeshAndMaterial* mesh : meshes)
		{
			mesh->mesh->UpdateVertices();
		}
	}

	void SoftBodyForVertex::UpdatModelVertices()
	{
		EnterCriticalSection(mCriticalSection);

		for (Node* node : mListOfNodes)
		{
			node->mPointerToVertex->positions = glm::inverse(transform.GetTransformMatrix()) * glm::vec4(node->mCurrentPosition, 1.0f);
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

		for (Node* node : mListOfNodes)
		{
			Renderer::GetInstance().DrawSphere(node->mCurrentPosition, mNodeDrawRadius, nodeColor);
		}

		for (LockNode& node : mListOfLockNodes)
		{
			Renderer::GetInstance().DrawSphere(node.center, node.radius, lockNodeColor);
		}


		for (Stick* stick : mListOfSticks)
		{
			Renderer::GetInstance().DrawLine(stick->mNodeA->mCurrentPosition, stick->mNodeB->mCurrentPosition, stickColor);
		}
	}

	void SoftBodyForVertex::OnPropertyDraw()
	{
		Model::OnPropertyDraw();

		if (!ImGui::TreeNodeEx("SoftBody", ImGuiTreeNodeFlags_DefaultOpen))
		{
			return;
		}

		ImGuiUtils::DrawBool("ShowDebug", showDebugModels);

		ImGui::TreePop();

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

}
