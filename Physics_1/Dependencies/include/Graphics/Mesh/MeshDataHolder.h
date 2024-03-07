#pragma once

#include "../Buffer/Vertex.h"
#include <vector>

class MeshDataHolder
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

