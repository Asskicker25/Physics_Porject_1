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
			PointerToVertex(Vertex& vertex)
			{
				mLocalVertex = vertex;
				mPointerToVertex = &vertex;
			};

			Vertex mLocalVertex;
			Vertex* mPointerToVertex = nullptr;
		};

		struct PointerToIndex
		{
			PointerToIndex(unsigned int& index)
			{
				mLocalIndex = index;
				mPointerToIndex = &index;
			}

			unsigned int mLocalIndex;
			unsigned int* mPointerToIndex = nullptr;
		};

		

		SoftBody() = default;

		void InitializeSoftBody();


	private:
		void SetupNodes();
		void SetupSticks();

		std::vector<PointerToVertex> mListOfVertices;
		std::vector<PointerToIndex> mListOfIndices;

		std::vector<Node*> mListOfNodes;
		std::vector<Stick*> mListOfSticks;
	};

}
