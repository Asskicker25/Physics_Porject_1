#pragma once

#include "Mesh.h"
#include "../Transform.h"
#include "../Material/BaseMaterial.h"
#include "MeshDataHolder.h"
#include "../EntityManager/Entity.h"

#include "../Texture/Texture.h"

#define NOMINMAX
#include <Windows.h>

#include <functional>



class Renderer;
class ModelAsyncManager;
struct ModelThreadInfo;


struct MeshAndMaterial
{
	std::shared_ptr<Mesh> mesh;
	BaseMaterial* material;
};


extern DWORD WINAPI LoadModelInThread(LPVOID lpParameter);

class Model : public Entity
{
public: 
	Model();
	Model(const Model& model);
	~Model();
	Model(const std::string& path, bool debugModel = false);

	void Draw(Shader* shader);
	void DrawSolidColor(Shader* shader, glm::vec3 color);
	Model* CopyFromModel(const Model& model, bool initialize = false);
	//Material material;

	Transform* GetTransform();
	void LoadModel(const std::string& path, bool debugModel = false);
	void LoadModel(MeshDataHolder& meshData, bool debugModel = false);
	void ProcessNode(aiNode* node, const aiScene* scene);

	void LoadModelAsync(const std::string& path, bool debugModel = false);

	void LoadFromMeshDataHolders();

	void DrawNormals();
	virtual void DrawShaded(Shader* shader);
	void DrawWireframe(const glm::vec3& color);


	virtual void DrawShaded(MeshAndMaterial* mesh, Shader* shader);

	virtual void OnPropertyDraw();
	virtual void OnSceneDraw();
	virtual void Render();

	// Inherited via Entity
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void OnDestroy();

	std::string modelId;

	bool isLoaded = false;
	bool isMeshDataReceived = false;
	bool isDebugModel = false;
	bool isActive = true;
	bool isWireframe = false;
	bool isGizmoItem = false;
	bool applyGizmoScale = true;

	std::vector<MeshAndMaterial*> meshes;
	std::vector<ImageHolder*> texturesLoaded;
	std::string directory;

	Shader* shader;

	ModelThreadInfo* modelThreadInfo = nullptr;

	std::function<void(Model*)> OnModelLoaded = nullptr;

protected:


	virtual MeshDataHolder* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	ImageHolder* LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	ImageHolder* LoadDefaultMaterialTextures(aiTextureType type, std::string typeName);

	void HandleMeshDataLoading();
	void DrawWireframe(MeshAndMaterial* mesh, Shader* shader);
	void DrawNormals(MeshAndMaterial* mesh, Shader* shader);

	void SetModelMatrix(Shader* shader);
	void SetInverseModelMatrix(Shader* shader);

	void OnModelSelected();

	std::vector<MeshDataHolder*> mListOfMeshDataHolders;
	
};



struct ModelThreadInfo
{
	Model* mModel = nullptr;
	ModelAsyncManager* mModelAsyncManager = nullptr;

	DWORD mThreadId = 0;
	HANDLE mThreadHandle = 0;

	bool mDebugModel = false;
	bool mIsRunning = false;

	float mSleepTime = 1;

	std::string mModelPath;

	CRITICAL_SECTION* mCriticalSection = nullptr;

};