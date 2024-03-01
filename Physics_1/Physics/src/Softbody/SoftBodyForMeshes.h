#pragma once
#include "BaseSoftBody.h"

namespace Verlet
{
	class SoftBodyForMeshes : public BaseSoftBody
	{
	public:
		SoftBodyForMeshes();
		~SoftBodyForMeshes();

		virtual void InitializeSoftBody();

		virtual void UpdateSoftBody(float deltaTime, CRITICAL_SECTION& criticalSection);
		virtual void Render();
		virtual void OnPropertyDraw();

		virtual void UpdateModelVertices();
		virtual void UpdateModelNormals();

		void AddForceToRandomNode(glm::vec3 velocity);

		void AddStickBetweenNodeIndex(unsigned int nodeA, unsigned int nodeB);
	
		//After Initialized
		void LockNodeAtIndex(int index);
		void InitializeLockNodes(std::vector<unsigned int> indexToLock);

		CRITICAL_SECTION* mCriticalSection;

	private:
		void SetupNodes();
		void SetupSticks();

		bool IsNodeLocked(unsigned int& currentIndex);

		std::vector<MeshHolder> mListOfMeshes;

		std::vector<Node*> mListOfLockedNodes;
		std::vector<unsigned int > mIndexesToLock;

	};

}
