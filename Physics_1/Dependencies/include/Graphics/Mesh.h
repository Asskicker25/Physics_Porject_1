#include "Debugger.h"
#include "Vertex.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "TextureData.h"
#include "Material.h"


#pragma once
class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector< unsigned int> indices;
	std::vector< Texture*> textures;
	Material material;

	Mesh();
	Mesh(std::vector<Vertex> &vertices, std::vector< unsigned int> &indices, std::vector< Texture*> textures);
	void DrawMesh(Shader& shader, bool loadMaterials = true, bool isWireFrame = false);
	VertexArray VAO;

private:
	VertexBuffer VBO;
	IndexBuffer IBO;
	VertexLayout layout;

	//unsigned int VAO, VBO, EBO;

	void SetupMesh();
};
