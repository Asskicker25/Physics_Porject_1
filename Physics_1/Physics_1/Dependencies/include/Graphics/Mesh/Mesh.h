#pragma once

#include "../Object.h"
#include "../Buffer/Vertex.h"
#include "../Shaders/Shader.h"
#include "../Buffer/VertexArray.h"
#include "../Buffer/VertexBuffer.h"
#include "../Buffer/VertexLayout.h"
#include "../Texture/BaseTexture.h"
#include "../Buffer/IndexBuffer.h"
#include "../Texture/TextureData.h"
#include "../Material/Material.h"
#include "../Buffer/Triangle.h"

class Mesh : public Object
{
public:
	std::vector<Vertex> vertices;
	std::vector< unsigned int> indices;
	std::vector< Triangles > triangles;
	std::vector< BaseTexture* > textures;

	Mesh() = default;
	Mesh(std::vector<Vertex>& vertices, std::vector< unsigned int>& indices, std::string name = "NoName");

	virtual void DrawShadedMesh(Shader* shader, BaseMaterial* material,  bool isWireFrame = false);
	void DrawSolidColorMesh(Shader* shader, glm::vec3 color, bool isWireFrame = false);
	void DrawNormals(Shader* shader, glm::vec3 color, glm::mat4 transformMatrix);

	void UpdateVertices();
	void UpdateVertices(std::vector<Vertex>& vertices, std::vector< unsigned int> indices);

	VertexArray VAO;
	IndexBuffer IBO;

	std::string name;

	static float normalScale;

protected:

	VertexBuffer VBO;
	VertexLayout layout;

	//unsigned int VAO, VBO, EBO;

	virtual void SetupMesh();
	void CalculateTriangles();

	// Inherited via Object
	void OnPropertyDraw() override;

	// Inherited via Object
	void OnSceneDraw() override;
};

