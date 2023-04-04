#include "Assistant.h"
#include "Core.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FAssistantModule"
DEFINE_LOG_CATEGORY(LogAssistant);

void* MemoryManagerWrapper::AllocateMemory(std::size_t blockSize, std::size_t /*alignment*/, const char* /*allocationTag*/)
{
    return ::operator new(blockSize);
}

void MemoryManagerWrapper::FreeMemory(void* memoryPtr)
{
    return ::operator delete(memoryPtr);
}

FAssistantModule::FAssistantModule()
:
    m_sdkOptions(new Aws::SDKOptions)
{
}

FAssistantModule::~FAssistantModule()
{
    delete static_cast<Aws::SDKOptions *>(m_sdkOptions);
    m_sdkOptions = nullptr;
}

void FAssistantModule::StartupModule()
{
    Aws::SDKOptions* awsSDKOptions = static_cast<Aws::SDKOptions*>(m_sdkOptions);
    awsSDKOptions->memoryManagementOptions.memoryManager = &m_memoryManager;
    Aws::InitAPI(*awsSDKOptions);
    m_apiInitialized = true;
}

void FAssistantModule::ShutdownModule()
{
    if (!m_apiInitialized) {
        return;
    }
    m_apiInitialized = false;
    Aws::ShutdownAPI(*static_cast<Aws::SDKOptions *>(m_sdkOptions));
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_PRIMARY_GAME_MODULE( FAssistantModule, Assistant, "Assistant" );
