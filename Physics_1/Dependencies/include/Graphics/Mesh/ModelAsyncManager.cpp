#include "ModelAsyncManager.h"

ModelAsyncManager::~ModelAsyncManager()
{
}

ModelAsyncManager& ModelAsyncManager::GetInstance()
{
    static ModelAsyncManager instance;
    return instance;
}

void ModelAsyncManager::AddModel(Model* model)
{
    HandleCritSectionInitializing();

    EnterCriticalSection(&critSection);

    model->modelThreadInfo->mModelAsyncManager = this;

    model->modelThreadInfo->mCriticalSection = &critSection;

    mListOfModelsToLoad.push_back(model);

    mListOfModelsToLoad[0]->modelThreadInfo->mIsRunning = true;

    LeaveCriticalSection(&critSection);
}

void ModelAsyncManager::OnModelLoadDone()
{
    EnterCriticalSection(&critSection);

    Model* loadedModel = mListOfModelsToLoad[0];

    loadedModel->isMeshDataReceived = true;
    //WaitForSingleObject(loadedModel->modelThreadInfo->mThreadHandle, INFINITE);
    CloseHandle(loadedModel->modelThreadInfo->mThreadHandle);

    mListOfModelsToLoad.erase(mListOfModelsToLoad.begin());

    if (mListOfModelsToLoad.size() != 0)
    {
        mListOfModelsToLoad[0]->modelThreadInfo->mIsRunning = true;
    }

    LeaveCriticalSection(&critSection);
}

void ModelAsyncManager::HandleCritSectionInitializing()
{
    if(mCritInitialized == true) return;

    InitializeCriticalSection(&critSection);
}
