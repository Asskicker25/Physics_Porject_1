#pragma once

#include "../Buffer/Vertex.h"
#include <vector>
#include "../Texture/Image.h"

struct ImageHolder
{
    ~ImageHolder();


    Image image;
    std::string path;
    std::string type;
};

class MeshDataHolder
{
public:

    ~MeshDataHolder();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    ImageHolder* diffuseTexture = nullptr;
    ImageHolder* specularTexture = nullptr;
    ImageHolder* alphaTexture = nullptr;

    bool useMaskTexture = false;
    bool hasMaterials = false;

    std::string meshName;
};

