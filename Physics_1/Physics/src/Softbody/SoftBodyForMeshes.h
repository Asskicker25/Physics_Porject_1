#pragma once
#include "BaseSoftBody.h"

namespace Verlet
{
	class SoftBodyForMeshes : public BaseSoftBody
	{
	public:
		struct Node;
		struct Stick;

		SoftBodyForMeshes();
		~SoftBodyForMeshes();

		virtual void InitializeSoftBody();

		virtual void UpdateSoftBody(float deltaTime, CRITICAL_SECTION& criticalSection);
		virtual void Render();
		virtual void OnPropertyDraw();

		void AddForceToRandomNode(glm::vec3 velocity);

		void UpdateModelData(float deltaTime);
		void UpdateBufferData() override;

		void AddStickBetweenNodeIndex(unsigned int nodeA, unsigned int nodeB);
	
		//After Initialized
		void LockNodeAtIndex(int index);
		void InitializeLockNodes(std::vector<unsigned int> indexToLock);

		CRITICAL_SECTION* mCriticalSection;

	private:
		void SetupNodes();
		void SetupSticks();

		void UpdateNodePosition(float deltaTime);
		void UpdatePositionByVerlet(Node*, float deltaTime);
		void SatisfyConstraints(float deltaTime);

		void UpdateModelVertices();
		void UpdateModelNormals();

		bool IsNodeLocked(unsigned int& currentIndex);

		const glm::vec4 nodeColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		const glm::vec4 stickColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);


		std::vector<MeshHolder> mListOfMeshes;

		std::vector<Node*> mListOfNodes;
		std::vector<Stick*> mListOfSticks;
		std::vector<Node*> mListOfLockedNodes;
		std::vector<unsigned int > mIndexesToLock;

	};

}
