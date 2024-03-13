#include "Model.h"
#include "../Renderer.h"
#include "../Material/UnlitColorMaterial.h"
#include "../Material/Material.h"
#include "../Panels/EditorLayout.h"
#include "ModelAsyncManager.h"

Model::Model()
{
	shader = Renderer::GetInstance().defaultShader;
}

Model::Model(const Model& model)
{
	isActive = model.isActive;
	meshes = model.meshes;
	directory = model.directory;
	transform = model.transform;
	shader = model.shader;
	//material = model.material;

}


Model::~Model()
{
	meshes.clear();
}

Model::Model(const std::string& path, bool debugModel)
{
	shader = Renderer::GetInstance().defaultShader;

	LoadModel(path, debugModel);
}

void Model::Draw(Shader* shader)
{
	if (modelThreadInfo != nullptr)
	{
		EnterCriticalSection(modelThreadInfo->mCriticalSection);

		if (!isMeshDataReceived) 
		{
			LeaveCriticalSection(modelThreadInfo->mCriticalSection);
			return;
		}
		LeaveCriticalSection(modelThreadInfo->mCriticalSection);
	}

	HandleMeshDataLoading();

	if (!isActive) return;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		if (isWireframe)
		{
			DrawWireframe(meshes[i], Renderer::GetInstance().solidColorShader);
			continue;
		}

		switch (Renderer::GetInstance().renderMode)
		{
		case SHADED:

			DrawShaded(meshes[i], shader);

			break;
		case WIREFRAME:

			DrawWireframe(meshes[i], Renderer::GetInstance().solidColorShader);

			break;

		case SHADED_WIREFRAME:

			DrawShaded(meshes[i], shader);

			DrawWireframe(meshes[i], Renderer::GetInstance().solidColorShader);
			break;
		}

		/*	if (!renderer->showNormals) continue;

			DrawNormals(meshes[i], renderer->solidColorShader);*/
	}


}

void Model::DrawSolidColor(Shader* shader, glm::vec3 color)
{
	if (!isActive) return;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		shader->Bind();
		SetModelMatrix(shader);

		meshes[i]->mesh->DrawSolidColorMesh(shader, color);

	}
}

void Model::DrawShaded(MeshAndMaterial* mesh, Shader* shader)
{
	shader->Bind();

	if (shader->applyModel)
	{
		SetModelMatrix(shader);
	}

	if (shader->applyInverseModel)
	{
		SetInverseModelMatrix(shader);
	}

	mesh->mesh->DrawShadedMesh(shader, mesh->material, false);

}

void Model::HandleMeshDataLoading()
{
	if (isLoaded) return;
	if (!isMeshDataReceived) return;

	LoadFromMeshDataHolders();
}

void Model::DrawWireframe(MeshAndMaterial* mesh, Shader* shader)
{
	shader->Bind();
	SetModelMatrix(shader);

	mesh->mesh->DrawSolidColorMesh(Renderer::GetInstance().solidColorShader,
		Renderer::GetInstance().wireframeMaterial->GetBaseColor(), true);
}


void Model::DrawNormals(MeshAndMaterial* mesh, Shader* shader)
{
	shader->Bind();
	SetModelMatrix(shader);

	mesh->mesh->DrawNormals(shader, Renderer::GetInstance().normalsMaterial->GetBaseColor(), transform.GetTransformMatrix());
}

void Model::SetModelMatrix(Shader* shader)
{
	shader->SetUniformMat("model", transform.GetTransformMatrix());
}

void Model::SetInverseModelMatrix(Shader* shader)
{
	shader->SetUniformMat("inverseModel", transform.GetInverseMatrix());
}


void Model::DrawNormals()
{
	if (!isActive) return;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		DrawNormals(meshes[i], Renderer::GetInstance().solidColorShader);
	}
}

void Model::DrawShaded(Shader* shader)
{
	if (!isActive) return;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		DrawShaded(meshes[i], shader);
	}
}

void Model::DrawWireframe(const glm::vec3& color)
{
	Renderer::GetInstance().solidColorShader->Bind();
	SetModelMatrix(Renderer::GetInstance().solidColorShader);

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->mesh->DrawSolidColorMesh(Renderer::GetInstance().solidColorShader, color, true);
	}

}


Model* Model::CopyFromModel(const Model& model, bool initialize)
{
	isActive = model.isActive;
	directory = model.directory;
	transform = model.transform;
	shader = model.shader;
	isGizmoItem = model.isGizmoItem;
	applyGizmoScale = model.applyGizmoScale;
	//material = model.material;

	for (MeshAndMaterial* mesh : model.meshes)
	{
		MeshAndMaterial* newMesh = new MeshAndMaterial();

		newMesh->mesh = mesh->mesh;
		newMesh->material = mesh->material->CloneMaterial();

		meshes.push_back(newMesh);
	}

	if (initialize)
	{
		InitializeEntity(this);
		Renderer::GetInstance().AddModel(this);
	}

	isLoaded = true;

	return this;
}


void Model::LoadModel(MeshDataHolder& meshData, bool debugModel)
{
	isDebugModel = debugModel;

	UnlitColorMaterial* meshMat = new UnlitColorMaterial();

	meshes.push_back(new MeshAndMaterial
		{ std::make_shared<Mesh>(meshData.vertices, meshData.indices), meshMat });

	isLoaded = true;

	if (debugModel) return;

	InitializeEntity(this);

}

void Model::LoadModelAsync(const std::string& path, bool debugModel)
{
	isDebugModel = debugModel;

	modelThreadInfo = new ModelThreadInfo();
	modelThreadInfo->mModelPath = path;
	modelThreadInfo->mDebugModel = debugModel;
	modelThreadInfo->mModel = this;
	isLoaded = false;

	ModelAsyncManager::GetInstance().AddModel(this);

	modelThreadInfo->mThreadHandle = CreateThread(
		NULL,							// lpThreadAttributes,
		0,								// dwStackSize,
		LoadModelInThread,				// lpStartAddress,
		(void*)modelThreadInfo,			//  lpParameter,
		0,								// dwCreationFlags (0 or CREATE_SUSPENDED)
		&modelThreadInfo->mThreadId		// lpThreadId
	);

	if (!debugModel)
	{
		Renderer::GetInstance().AddModel(this);
	}
}


DWORD LoadModelInThread(LPVOID lpParameter)
{
	ModelThreadInfo* threadInfo = (ModelThreadInfo*)lpParameter;

	while (!threadInfo->mModel->isMeshDataReceived)
	{

		//EnterCriticalSection(threadInfo->mCriticalSection);

		if (threadInfo->mIsRunning)
		{
			std::ifstream file(threadInfo->mModelPath);

			if (!file.good()) { Debugger::Print("File Path Not Found : ", threadInfo->mModelPath); }

			Assimp::Importer import;

			const aiScene * scene = import.ReadFile(threadInfo->mModelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;

				threadInfo->mModelAsyncManager->OnModelLoadDone();

				return 0;
			}

			threadInfo->mModel->directory = threadInfo->mModelPath.substr(0, threadInfo->mModelPath.find_last_of('/'));

			threadInfo->mModel->ProcessNode(scene->mRootNode, scene);

			Sleep(threadInfo->mSleepTime);

			threadInfo->mModelAsyncManager->OnModelLoadDone();

		}

		//LeaveCriticalSection(threadInfo->mCriticalSection);

		Sleep(threadInfo->mSleepTime);
	}


	return 0;
}


Transform* Model::GetTransform()
{
	return &transform;
}


void Model::LoadModel(const std::string& path, bool debugModel)
{
	isDebugModel = debugModel;

	std::ifstream file(path);

	if (!file.good()) { Debugger::Print("File Path Not Found : ", path); }

	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
	LoadFromMeshDataHolders();

	isLoaded = true;


	if (debugModel) return;

	InitializeEntity(this);
	Renderer::GetInstance().AddModel(this);

}



void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		//Mesh* myMesh = new Mesh(ProcessMesh(mesh, scene));
		mListOfMeshDataHolders.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}


MeshDataHolder* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	MeshDataHolder* meshDataHolder = new MeshDataHolder();

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

		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		meshDataHolder->vertices.push_back(vertex);
	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			meshDataHolder->indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		meshDataHolder->hasMaterials = true;

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		meshDataHolder->diffuseTexture = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		meshDataHolder->specularTexture = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		meshDataHolder->alphaTexture = LoadMaterialTextures(material, aiTextureType_OPACITY, "texture_opacity");

		if (material->GetTextureCount(aiTextureType_OPACITY) != 0)
		{
			meshDataHolder->useMaskTexture = true;
		}
	}
	else
	{
		meshDataHolder->hasMaterials = false;
	}

	std::string meshName = mesh->mName.C_Str();

	if (meshName == "")
	{
		meshDataHolder->meshName = "Mesh " + std::to_string(meshes.size());
	}

	return meshDataHolder;
}


ImageHolder* Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{

	if (mat->GetTextureCount(type) == 0)
	{
		return LoadDefaultMaterialTextures(type, typeName);
	}


	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString texString;

		mat->GetTexture(type, i, &texString);

		for (unsigned int i = 0; i < texturesLoaded.size(); i++)
		{
			if (std::strcmp(texturesLoaded[i]->path.data(), texString.C_Str()) == 0)
			{
				return texturesLoaded[i];
			}
		}
		std::string filename = std::string(texString.C_Str());
		filename = directory + '/' + filename;

		if (fileExists(filename))
		{
			ImageHolder* imageHolder = new ImageHolder();

			Texture::LoadTextureImage(filename.c_str(), imageHolder->image);

			imageHolder->type = typeName;
			imageHolder->path = filename;
			texturesLoaded.push_back(imageHolder);

			return imageHolder;
		}
		else
		{
			return LoadDefaultMaterialTextures(type, typeName);
		}
	}
}


ImageHolder* Model::LoadDefaultMaterialTextures(aiTextureType type, std::string typeName)
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
	case aiTextureType_OPACITY:
		path = "res/Textures/DefaultTextures/Opacity_Default.png";
		break;
	}

	ImageHolder* imageHolder = new ImageHolder();
	Texture::LoadTextureImage(path.c_str(), imageHolder->image);

	imageHolder->type = typeName;
	imageHolder->path = path;
	texturesLoaded.push_back(imageHolder);

	return imageHolder;
}


void Model::LoadFromMeshDataHolders()
{
	for (MeshDataHolder* meshData : mListOfMeshDataHolders)
	{
		if (meshData->hasMaterials)
		{
			Material* mat = new Material();
			mat->diffuseTexture = new Texture(meshData->diffuseTexture->image, meshData->diffuseTexture->path, meshData->diffuseTexture->type);
			mat->specularTexture = new Texture(meshData->specularTexture->image, meshData->specularTexture->path, meshData->specularTexture->type);
			mat->alphaMask = new Texture(meshData->alphaTexture->image, meshData->alphaTexture->path, meshData->alphaTexture->type);
			mat->useMaskTexture = meshData->useMaskTexture;

			mat->SetBaseColor(glm::vec4(1));

			meshes.push_back(new MeshAndMaterial
				{ std::make_shared<Mesh>(meshData->vertices, meshData->indices, meshData->meshName), mat });
		}
		else
		{
			UnlitColorMaterial* mat = new UnlitColorMaterial();
			mat->SetBaseColor(glm::vec4(1));

			meshes.push_back(new MeshAndMaterial
				{ std::make_shared<Mesh>(meshData->vertices, meshData->indices, meshData->meshName), mat });
		}

	}

	/*while (mListOfMeshDataHolders.size() != 0)
	{
		delete mListOfMeshDataHolders[0];
	}*/
	mListOfMeshDataHolders.clear();

	isLoaded = true;

	if (OnModelLoaded != nullptr)
	{
		OnModelLoaded(this);
	}

	if (isDebugModel) return;

	InitializeEntity(this);
	//Renderer::GetInstance().AddModel(this);
}


void Model::Start()
{
}

void Model::Update(float deltaTime)
{
	
	
}

void Model::OnDestroy()
{
	Renderer::GetInstance().RemoveModel(this);
}

void Model::OnSceneDraw()
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
	node_flags |= ImGuiTreeNodeFlags_SpanFullWidth;

	if (isSelected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	ImGui::PushStyleColor(ImGuiCol_Text, isActive ? ImVec4(1, 1, 1, 1) : ImVec4(0.4f, 0.4, 0.4, 1));

	bool node_open = ImGui::TreeNodeEx(name.c_str(), node_flags);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		OnModelSelected();
	}

	if (node_open)
	{
		for (MeshAndMaterial* mesh : meshes)
		{
			ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf;
			leaf_flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
			leaf_flags |= ImGuiTreeNodeFlags_SpanFullWidth;

			if (mesh->mesh->isSelected)
			{
				leaf_flags |= ImGuiTreeNodeFlags_Selected;
			}

			ImGui::TreeNodeEx(mesh->mesh->name.c_str(), leaf_flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				Renderer::GetInstance().SetSelectedModel(this);
				EditorLayout::GetInstance().SetSelectedObjects({ mesh->mesh.get(), mesh->material });
			}
		}

		ImGui::TreePop();
	}

	ImGui::PopStyleColor();
}

void Model::OnPropertyDraw()
{
	ImGui::Checkbox("###ISActive", &isActive);
	ImGui::SameLine();
	Entity::OnPropertyDraw();
}


void Model::OnModelSelected()
{
	Renderer::GetInstance().SetSelectedModel(this);
	EditorLayout::GetInstance().SetSelectedObjects({ this });
}

void Model::Render()
{
}

