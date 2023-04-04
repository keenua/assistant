#include "Assistant.h"
#include "Core.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FAssistantModule"
DEFINE_LOG_CATEGORY(LogAssistant);

FAssistantModule::FAssistantModule()
{
}

FAssistantModule::~FAssistantModule()
{
}

void FAssistantModule::StartupModule()
{
}

void FAssistantModule::ShutdownModule()
{
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_PRIMARY_GAME_MODULE( FAssistantModule, Assistant, "Assistant" );
