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

	struct Stick;

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

		Node(std::vector<PointerToVertex>& vertex, glm::mat4& transformMat, float radius,
			bool isLocked = false)
		{
			mIsLocked = isLocked;
			mRadius = radius;

			mCurrentPosition = CalculatePosition(vertex);
			InitializeVertexPointer(vertex, mCurrentPosition);

			mCurrentPosition = transformMat * glm::vec4(mCurrentPosition, 1.0f);
			mOldPositionm = mCurrentPosition;
		}

		~Node()
		{
			mListOfConnectedSticks.clear();
			mPointerToVertices.clear();
		}

		bool mIsLocked = false;
		float mRadius = 0;

		glm::vec3 mCurrentPosition = glm::vec3(0);
		glm::vec3 mOldPositionm = glm::vec3(0);
		glm::vec3 velocity = glm::vec3(0);

		std::vector<PointerToVertex> mPointerToVertices;
		std::vector<Stick*> mListOfConnectedSticks;
		std::vector<PointerToIndex> mListOfIndexes;
	};


	struct Stick
	{
		Stick(Node* nodeA, Node* nodeB)
		{
			mNodeA = nodeA;
			mNodeB = nodeB;

			mRestLength = glm::distance(nodeA->mCurrentPosition, nodeB->mCurrentPosition);

			nodeA->mListOfConnectedSticks.push_back(this);
			nodeB->mListOfConnectedSticks.push_back(this);
		};

		~Stick()
		{
			mNodeA = nullptr;
			mNodeB = nullptr;
		}

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

	virtual void AddCollidersToCheck(PhysicsObject* phyObj);
	virtual void SetNodeRadius(int index, float radius);

	virtual void DisconnectStick(Stick* stick);

	bool showDebugModels = true;
	glm::vec3 mGravity = glm::vec3(0);
	unsigned int mNumOfIterations = 10;

	float mNodeRadius = 0.1f;
	float mTightness = 1.0f;
	float mBounceFactor = 1.0f;

	glm::vec3 mNodeMaxVelocity = glm::vec3(10);

	std::vector<PhysicsObject*> mListOfCollidersToCheck;

	std::vector<Node*> mListOfNodes;
	std::vector<Stick*> mListOfSticks;

	CollisionMode collisionMode = CollisionMode::SOLID;

protected:
	void CleanZeros(glm::vec3& value);
	

	const glm::vec4 nodeColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	const glm::vec4 stickColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	
};

