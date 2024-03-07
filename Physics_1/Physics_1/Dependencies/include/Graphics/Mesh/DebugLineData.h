#pragma once

#include "../Debugger.h"
#include "../Buffer/Vertex.h"
#include "MeshDataHolder.h"

class DebugLineData : public MeshDataHolder
{
public:

	DebugLineData();

	void SetStartPoint(glm::vec3 startPoint);
	void SetEndPoint(glm::vec3 endPoint);
};



