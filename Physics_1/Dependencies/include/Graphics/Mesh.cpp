#include "Mesh.h"

int count = 0;

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture*> textures)
{
	VAO.Bind();

	this->vertices = vertices;
	this->indices = indices;

	if (textures.size() != 0)
	{
		material.diffuseTexture = textures[0];
		material.specularTexture = textures[1];
	}

	//this->textures = textures;

	SetupMesh();
}

void Mesh::DrawMesh(Shader& shader, bool loadMaterials)
{
	//VAO.Bind();

	if (loadMaterials)
	{
		shader.SetUniform3f("material.baseColor", material.GetBaseColor().x, material.GetBaseColor().y, material.GetBaseColor().z);
		shader.SetUniform3f("material.ambientColor", material.GetAmbientColor().x, material.GetAmbientColor().y, material.GetAmbientColor().z);
		shader.SetUniform1f("material.specularValue", material.GetSpecularValue());
		shader.SetUniform1f("material.shininess", material.shininess);

		if (material.diffuseTexture != nullptr)
		{
			material.diffuseTexture->SetTextureSlot(0);
			shader.SetUniform1i("texture_diffuse1", 0);
			material.diffuseTexture->Bind();
		}

		if (material.specularTexture != nullptr)
		{
			material.specularTexture->SetTextureSlot(1);
			shader.SetUniform1i("texture_specular1", 1);
			material.specularTexture->Bind();
		}
	}
	

	VAO.Bind();
	//std::cout << count << std:: endl;
	GLCALL(glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr));
	VAO.UnBind();
	count++;
}

void Mesh::SetupMesh()
{
	VAO.Bind();
	VBO.Setup(vertices.size() * sizeof(Vertex), &vertices[0]);
	IBO.Setup(indices.size(), &indices[0]);

	//Position
	layout.AddLayout<float>(3);

	//Normals
	layout.AddLayout<float>(3);

	//Texture
	layout.AddLayout<float>(2);

	VAO.AddBuffer(VBO, layout);

	VAO.UnBind();
}



