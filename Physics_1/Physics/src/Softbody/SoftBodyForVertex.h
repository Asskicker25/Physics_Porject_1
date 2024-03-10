#pragma once
#include "BaseSoftBody.h"

namespace Verlet
{

	class SoftBodyForVertex : public BaseSoftBody
	{
	public:
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

		virtual void UpdateSoftBody(float deltaTine);
		virtual void Render();
		virtual void OnPropertyDraw();


		virtual void UpdateModelVertices();
		virtual void UpdateModelNormals();

		void AddForceToRandomNode(glm::vec3 velocity);

		void DisconnectRandomStick();
		void DisconnectRandomNode();

		void AddLockNode(glm::vec3 posOffset, float radius);

		bool IsNodeLocked(Node* node);

		float mLockAffectDisatance = 0.0f;


	private:
		void SetupNodes();
		void SetupSticks();

		

		const glm::vec4 lockNodeColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

		std::vector<PointerToVertex> mListOfVertices;
		std::vector<PointerToIndex> mListOfIndices;
		std::vector<LockNode> mListOfLockNodes;				//Position Offset from center that calculates which nodes to lock based on radius
		

protected:
		std::vector<Node*> mListOfLockedNodes;

	};

}
