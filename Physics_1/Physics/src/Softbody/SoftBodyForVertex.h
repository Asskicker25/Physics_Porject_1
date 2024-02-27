#pragma once
#include "BaseSoftBody.h"

namespace Verlet
{

	class SoftBodyForVertex : public BaseSoftBody
	{
	public:
		struct Node;
		struct Stick;

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


		SoftBodyForVertex();
		~SoftBodyForVertex();

		virtual void InitializeSoftBody();

		virtual void UpdateSoftBody(float deltaTine, CRITICAL_SECTION& criticalSection);
		virtual void Render();
		virtual void OnPropertyDraw();

		void AddForceToRandomNode(glm::vec3 velocity);

		void AddLockNode(glm::vec3 posOffset, float radius);
		void UpdateModelData(float deltaTime);
		void UpdateBufferData() override;
		
		float mLockAffectDisatance = 0.0f;

		CRITICAL_SECTION* mCriticalSection;


	private:
		void SetupNodes();
		void SetupSticks();

		void UpdateNodePosition(float deltaTime);
		void UpdatePositionByVerlet(Node* node, float deltaTime);
		void SatisfyConstraints(float deltaTime);

		void UpdatModelVertices();
		void UpdateModelNormals();

		bool IsNodeLocked(Node* node);


		const glm::vec4 nodeColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		const glm::vec4 stickColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		const glm::vec4 lockNodeColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

		std::vector<PointerToVertex> mListOfVertices;
		std::vector<PointerToIndex> mListOfIndices;
		std::vector<LockNode> mListOfLockNodes;				//Position Offset from center that calculates which nodes to lock based on radius

		std::vector<Node*> mListOfNodes;
		std::vector<Stick*> mListOfSticks;
		std::vector<Node*> mListOfLockedNodes;

	};

}
