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

	bool showDebugModels = true;
	glm::vec3 mGravity = glm::vec3(0);
	unsigned int mNumOfIterations = 10;

	float mNodeRadius = 0.1f;
	float mTightness = 1.0f;

	std::vector<PhysicsObject*> mListOfCollidersToCheck;

protected:
	void CleanZeros(glm::vec3& value);
	
};

