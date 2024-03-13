#pragma once
#include "Model.h"

class ModelAsyncManager
{
public:

	ModelAsyncManager() = default;
	~ModelAsyncManager();

	static ModelAsyncManager& GetInstance();
	
	void AddModel(Model* model);
	void OnModelLoadDone();

private:

	void HandleCritSectionInitializing();

	bool mCritInitialized = false;

	CRITICAL_SECTION critSection;

	std::vector<Model*> mListOfModelsToLoad;
};

