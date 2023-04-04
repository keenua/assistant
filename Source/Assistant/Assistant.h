// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAssistant, Log, All);

class FAssistantModule : public IModuleInterface
{
public:

    /**
     * Constructs a FAssistantModule instance.
     */
    FAssistantModule();

    /**
     * Destructs a FAssistantModule instance.
     */
    virtual ~FAssistantModule();

    /**
     * Called right after the module DLL has been loaded and the module object has been created
     * Load dependent modules here, and they will be guaranteed to be available during ShutdownModule. ie:
     *
     * FModuleManager::Get().LoadModuleChecked(TEXT("HTTP"));
     *
     * @see IModuleInterface for details
     */
    virtual void StartupModule() override;

    /**
     * Called before the module is unloaded, right before the module object is destroyed.
     * During normal shutdown, this is called in reverse order that modules finish StartupModule().
     * This means that, as long as a module references dependent modules in it's StartupModule(), it
     * can safely reference those dependencies in ShutdownModule() as well.
     *
     * @see IModuleInterface for details
     */
    virtual void ShutdownModule() override;

    virtual bool IsGameModule() const override
    {
        return true;
    }
};