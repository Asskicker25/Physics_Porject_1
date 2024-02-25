#pragma once
#include <Graphics/Model.h>

namespace Verlet
{

	class SoftBody : public Model
	{
	public:
		struct Node;
		struct Stick;

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

		struct LockNode
		{
			LockNode(glm::vec3 center, float radius)
			{
				this->center = center;
				this->radius = radius;
			}

			glm::vec3 center = glm::vec3(0);
			float radius = 1.0f;
		};



		SoftBody();

		void InitializeSoftBody();

		virtual void Update(float deltaTine);
		virtual void Render();
		virtual void OnPropertyDraw();

		void AddLockNode(glm::vec3 posOffset, float radius);

		bool showDebugModels = true;

		glm::vec3 mGravity = glm::vec3(0);

		float mNodeDrawRadius = 0.1f;
		float mTightness = 1.0f;

		unsigned int mNumOfIterations = 10;

	private:
		void SetupNodes();
		void SetupSticks();

		void UpdateNodePosition(float deltaTime);
		void UpdatePositionByVerlet(Node* node, float deltaTime);
		void SatisfyConstraints(float deltaTime);
		void UpdateModelData(float deltaTime);

		bool IsNodeLocked(Node* node);
		void CleanZeros(glm::vec3& value);


		const glm::vec4 nodeColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		const glm::vec4 lockNodeColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		const glm::vec4 stickColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		std::vector<PointerToVertex> mListOfVertices;
		std::vector<PointerToIndex> mListOfIndices;
		std::vector<LockNode> mListOfLockNodes;				//Position Offset from center that calculates which nodes to lock based on radius

		std::vector<Node*> mListOfNodes;
		std::vector<Stick*> mListOfSticks;

	};

}