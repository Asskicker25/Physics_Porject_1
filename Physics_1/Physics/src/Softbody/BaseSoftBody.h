#pragma once
#include <Graphics/Model.h>
#include "../PhysicsObject.h"

#define NOMINMAX
#include <Windows.h>

class BaseSoftBody : public Model
{
public:
	struct PointerToVertex
	{
		PointerToVertex(Vertex& vertex, glm::mat4& transformMatrix)
		{
			mLocalVertex = vertex;
			mLocalVertex.positions = transformMatrix * glm::vec4(mLocalVertex.positions, 1.0f);
			mPointerToVertex = &vertex;
		};

		Vertex mLocalVertex;
		Vertex* mPointerToVertex = nullptr;

		glm::vec3 mOffsetFromCenter = glm::vec3(0);
	};

	struct PointerToIndex
	{
		PointerToIndex(unsigned int& index, int addIndexOffset = 0)
		{
			mLocalIndex = index + addIndexOffset;
			mPointerToIndex = &index;
		}

		unsigned int mLocalIndex;
		unsigned int* mPointerToIndex = nullptr;
	};

	struct Node
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

	struct Stick
	{
		Stick(Node* nodeA, Node* nodeB)
		{
			mNodeA = nodeA;
			mNodeB = nodeB;

			mRestLength = glm::distance(nodeA->mCurrentPosition, nodeB->mCurrentPosition);
		};

		bool isConnected = true;
		float mRestLength = 0;

		Node* mNodeA = nullptr;
		Node* mNodeB = nullptr;
	};

	struct MeshHolder
	{
		MeshHolder(std::vector<PointerToVertex> vertices, std::vector<PointerToIndex> indices) :
			mListOfVertices{ vertices }, 
			mListOfIndices{ indices } {};

		std::vector<PointerToVertex> mListOfVertices;
		std::vector<PointerToIndex> mListOfIndices;
	};

	virtual void UpdateSoftBody(float deltaTime, CRITICAL_SECTION& criticalSection) = 0;
	virtual void UpdateBufferData() = 0;
	virtual void InitializeSoftBody() = 0;

	virtual void OnPropertyDraw();
	virtual void Render();

	bool showDebugModels = true;
	glm::vec3 mGravity = glm::vec3(0);
	unsigned int mNumOfIterations = 10;

	float mNodeRadius = 0.1f;
	float mTightness = 1.0f;

	std::vector<PhysicsObject*> mListOfCollidersToCheck;

protected:
	void CleanZeros(glm::vec3& value);
	std::vector<Node*> mListOfNodes;
	std::vector<Stick*> mListOfSticks;

	const glm::vec4 nodeColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	const glm::vec4 stickColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	
};

