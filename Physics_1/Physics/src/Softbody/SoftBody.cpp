#include "SoftBody.h"

namespace Verlet
{

	struct SoftBody::Node
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

		Vertex* mPointerToVertex = nullptr;
	};

	struct  SoftBody::Stick
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

	void SoftBody::InitializeSoftBody()
	{
		for (MeshAndMaterial* mesh : meshes)
		{

			for (Vertex& vertexInMesh : mesh->mesh->vertices)
			{
				mListOfVertices.push_back(vertexInMesh);
			}

			for (unsigned int& indexInMesh : mesh->mesh->indices)
			{
				mListOfIndices.push_back(indexInMesh);
			}
		}

		SetupNodes();
		SetupSticks();
	}

	void SoftBody::SetupNodes()
	{
		mListOfNodes.reserve(mListOfVertices.size());

		for (PointerToVertex& pos : mListOfVertices)
		{
			mListOfNodes.push_back(new Node(pos));
		}
	}

	void SoftBody::SetupSticks()
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
	}
}
