#include "Model.h"

Model::Model()
{
	loadMatProperties = true;
	loadTextures = true;
}

Model::Model(const Model& model)
{
	isActive = model.isActive;
	meshes = model.meshes;
	directory = model.directory;
	transform = model.transform;
	//material = model.material;
	loadTextures = model.loadTextures;
	loadMatProperties = model.loadMatProperties;
}


Model::~Model()
{
	//std::cout << "Model ID : " << modelId <<std::endl;
	/*while (meshes.size() > 0)
	{
		if (meshes[0] != nullptr)
		{
			delete meshes[0];
		}
		meshes.erase(std::remove(meshes.begin(), meshes.end(), meshes[0]), meshes.end());
	}*/
	meshes.clear();
}

Model::Model(std::string path, bool loadTextures, bool loadMatProperties)
{
	LoadModel(path, loadTextures, loadMatProperties);
}

void Model::Draw(Shader& shader)
{
	if (!isActive) return;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		shader.Bind();
		shader.SetUniformMat("model", transform.GetTransformMatrix());

		//if (loadMatProperties)
		//{
		//	//Material
		//	shader.SetUniform3f("material.baseColor", material.GetBaseColor().x, material.GetBaseColor().y, material.GetBaseColor().z);
		//	shader.SetUniform3f("material.ambientColor", material.GetAmbientColor().x, material.GetAmbientColor().y, material.GetAmbientColor().z);
		//	shader.SetUniform1f("material.specularValue", material.GetSpecularValue());
		//	shader.SetUniform1f("material.shininess", material.shininess);
		//}

		if (loadTextures)
		{
			shader.SetUniformMat("inverseModel", transform.GetInverseMatrix());
		}
		meshes[i]->DrawMesh(shader, loadMatProperties, isWireframe);
	}
}

void Model::LoadModel(std::string path, bool loadTextures, bool loadMatProperties)
{
	this->loadTextures = loadTextures;
	this->loadMatProperties = loadMatProperties;

	std::ifstream file(path);

	if (file.good())
	{
		Debugger::Print("File exits");
	}
	else
	{
		Debugger::Print("File doesn't exits");
	}

	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);

}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		//Mesh* myMesh = new Mesh(ProcessMesh(mesh, scene));
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	//std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 temp;

		temp.x = mesh->mVertices[i].x;
		temp.y = mesh->mVertices[i].y;
		temp.z = mesh->mVertices[i].z;

		vertex.positions = temp;

		if (mesh->HasNormals())
		{
			temp.x = mesh->mNormals[i].x;
			temp.y = mesh->mNormals[i].y;
			temp.z = mesh->mNormals[i].z;

			vertex.normals = temp;
		}

		/*if (mesh->mColors[0])
		{
			aiColor4D color = mesh->mColors[0][i];
			vertex.color = glm::vec4(color.r, color.g, color.b, color.a);
		}
		else
		{
			vertex.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}*/

		if (mesh->HasVertexColors(0))
		{
			aiColor4D color = mesh->mColors[0][i];
			vertex.color = glm::vec4(color.r, color.g, color.b, color.a);
		}
		else
		{
			vertex.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}


		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;

			/*temp.x = mesh->mTangents[i].x;
			temp.y = mesh->mTangents[i].y;
			temp.z = mesh->mTangents[i].z;

			vertex.tangents = temp;

			temp.x = mesh->mBitangents[i].x;
			temp.y = mesh->mBitangents[i].y;
			temp.z = mesh->mBitangents[i].z;*/

			//vertex.bitTangents = temp;

		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Material* meshMat = new Material();
	aiColor4D baseColor(1.0f, 1.0f, 1.0f, 1.0f);


	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &baseColor);
		
		if (loadTextures)
		{
			meshMat->diffuseTexture = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			meshMat->specularTexture = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		}
	}

	meshMat->SetBaseColor(glm::vec3(baseColor.r, baseColor.g, baseColor.b));


	return std::make_shared<Mesh>(vertices, indices, meshMat);
}


Texture* Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	//std::vector<Texture> textures;

	Debugger::Print(typeName + " count : ", mat->GetTextureCount(type));

	//std::cout << mat->GetTextureCount(type) << std::endl;

	if (mat->GetTextureCount(type) == 0)
	{
		return LoadDefaultMaterialTextures(type, typeName);
	}


	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString texString;

		mat->GetTexture(type, i, &texString);

		Debugger::Print("Tex path ", texString.C_Str());

		//std::cout << "Tex path " << texString.C_Str() << std::endl;

		for (unsigned int i = 0; i < texturesLoaded.size(); i++)
		{
			if (std::strcmp(texturesLoaded[i]->path.data(), texString.C_Str()) == 0)
			{
				return texturesLoaded[i];
			}
		}
		std::string filename = std::string(texString.C_Str());
		filename = directory + '/' + filename;

		if (Texture::fileExists(filename))
		{
			Texture* texture = new Texture(filename);

			texture->type = typeName;
			texture->path = texString.C_Str();
			texturesLoaded.push_back(texture);

			return texture;
		}
		else
		{
			return LoadDefaultMaterialTextures(type, typeName);
		}
	}
}

Texture* Model::LoadDefaultMaterialTextures(aiTextureType type, std::string typeName)
{
	std::string path = "";
	switch (type)
	{
	case aiTextureType_DIFFUSE:
		path = "res/Textures/DefaultTextures/Diffuse_Default.png";
		break;
	case aiTextureType_SPECULAR:
		path = "res/Textures/DefaultTextures/Specular_Default.jpg";
		break;
	}
	Texture* texture = new Texture(path);

	texture->type = typeName;
	texture->path = path;
	texturesLoaded.push_back(texture);

	return texture;
}

