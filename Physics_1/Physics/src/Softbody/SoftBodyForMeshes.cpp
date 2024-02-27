#include <Graphics/Renderer.h>
#include <Graphics/Panels/EditorLayout.h>
#include <Graphics/Panels/ImguiDrawUtils.h>
#include <Graphics/MathUtils.h>
#include "../PhysicsEngine.h"
#include "SoftBodyForMeshes.h"

#define NOMINMAX
#include <Windows.h>



namespace Verlet
{
	struct SoftBodyForMeshes::Node
	{

	private:

		glm::vec3 CalculatePosition(std::vector<PointerToVertex>& vertex)
		{
			glm::vec3 pos = glm::vec3(0);

			int i = 0;

			for (PointerToVertex& vert : vertex)
			{
				pos += vert.mPointerToVertex->positions;
				i++;
			}

			pos /= (float)i;

			return pos;
		}


		void InitializeVertexPointer(std::vector<PointerToVertex>& vertex, glm::vec3& center)
		{
			mPointerToVertices.reserve(vertex.size());

			for (PointerToVertex& vert : vertex)
			{
				vert.mOffsetFromCenter = vert.mPointerToVertex->positions - center;
				mPointerToVertices.push_back(vert);
			}
		}

	public:

		Node(std::vector<PointerToVertex>& vertex, glm::mat4& transformMat,
			bool isLocked = false)
		{
			mIsLocked = isLocked;

			mCurrentPosition = CalculatePosition(vertex);
			InitializeVertexPointer(vertex, mCurrentPosition);

			mCurrentPosition = transformMat * glm::vec4(mCurrentPosition, 1.0f);
			mOldPositionm = mCurrentPosition;
		}

		bool mIsLocked = false;
		glm::vec3 mCurrentPosition = glm::vec3(0);
		glm::vec3 mOldPositionm = glm::vec3(0);
		glm::vec3 velocity = glm::vec3(0);

		std::vector<PointerToVertex> mPointerToVertices;

	};

	struct SoftBodyForMeshes::Stick
	{

	};

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

	void SoftBodyForMeshes::UpdateSoftBody(float deltaTime, CRITICAL_SECTION& criticalSection)
	{
	}

	void SoftBodyForMeshes::AddForceToRandomNode(glm::vec3 velocity)
	{
	}

	void SoftBodyForMeshes::UpdateModelData(float deltaTime)
	{
	}

	void SoftBodyForMeshes::UpdateBufferData()
	{
	}

	void SoftBodyForMeshes::SetupNodes()
	{
		mListOfNodes.reserve(mListOfMeshes.size());
		glm::mat4 transformMat = transform.GetTransformMatrix();

		unsigned int i = 0;
		for (MeshHolder& mesh : mListOfMeshes)
		{

			Node* node = new Node(mesh.mListOfVertices,
				transformMat, IsNodeLocked(i));

			mListOfNodes.push_back(node);
			i++;
		}
	}

	void SoftBodyForMeshes::SetupSticks()
	{
	}

	void SoftBodyForMeshes::InitializeLockNodes(std::vector<unsigned int> indexToLock)
	{
		mIndexesToLock = indexToLock;
	}

	void SoftBodyForMeshes::LockNodeAtIndex(int index)
	{
		if (mListOfNodes.size() == 0) return;

		mListOfNodes[index]->mIsLocked = true;
	}

	bool SoftBodyForMeshes::IsNodeLocked(unsigned int& currentIndex)
	{
		for (unsigned int& index : mIndexesToLock)
		{
			if (index == currentIndex) return true;
		}

		return false;
	}

	void SoftBodyForMeshes::UpdateNodePosition(float deltaTime)
	{
	}

	void SoftBodyForMeshes::UpdatePositionsByVerlet(Node*, float deltaTime)
	{
	}

	void SoftBodyForMeshes::SatisfyConstraints(float deltaTime)
	{
	}

	void SoftBodyForMeshes::UpdateModelVertices()
	{
	}

	void SoftBodyForMeshes::UpdateModelNormals()
	{
	}


	void SoftBodyForMeshes::Render()
	{
		if (!showDebugModels) return;

		for (Node* node : mListOfNodes)
		{
			Renderer::GetInstance().DrawSphere(node->mCurrentPosition, mNodeDrawRadius, nodeColor);
		}


		/*for (Stick* stick : mListOfSticks)
		{
			Renderer::GetInstance().DrawLine(stick->mNodeA->mCurrentPosition, stick->mNodeB->mCurrentPosition, stickColor);
		}*/
	}

	void SoftBodyForMeshes::OnPropertyDraw()
	{
		Model::OnPropertyDraw();

		if (!ImGui::TreeNodeEx("SoftBody", ImGuiTreeNodeFlags_DefaultOpen))
		{
			return;
		}

		ImGuiUtils::DrawBool("ShowDebug", showDebugModels);

		ImGui::TreePop();

	}


}
