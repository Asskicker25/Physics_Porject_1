#pragma once
#include "Debugger.h"
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"


class Model
{
public: 
	Model();
	Model(const Model& model);
	~Model();
	Model(const std::string& path, bool loadTextures = true, bool loadMatProperties = true);

	void Draw(Shader* shader);
	void CopyFromModel(const Model& model);

	std::string modelId;
	bool isActive = true;
	bool isWireframe = false;

	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<Texture*> texturesLoaded;
	std::string directory;

	Transform transform;
	//Material material;

	void LoadModel(const std::string& path, bool loadTextures = true, bool loadMatProperties = true);

private:
	bool loadTextures;
	bool loadMatProperties;

	void ProcessNode(aiNode* node, const aiScene* scene);
	std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Texture* LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	Texture* LoadDefaultMaterialTextures(aiTextureType type, std::string typeName);
};

