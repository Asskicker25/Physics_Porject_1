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
#include "Triangle.h"

#pragma once
class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector< unsigned int> indices;
	std::vector< Triangles > triangles;
	std::vector< Texture*> textures;
	Material* material = nullptr;

	Mesh();
	Mesh(std::vector<Vertex>& vertices, std::vector< unsigned int>& indices, Material* mat);
	void DrawMesh(Shader& shader, bool loadMaterials = true, bool isWireFrame = false);
	VertexArray VAO;

private:
	VertexBuffer VBO;
	IndexBuffer IBO;
	VertexLayout layout;

	//unsigned int VAO, VBO, EBO;

	void SetupMesh();
	void CalculateTriangles();
};

