// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include <aws/core/Aws.h>

DECLARE_LOG_CATEGORY_EXTERN(LogAssistant, Log, All);

/**
 * Memory manager wrapper to provide the AWS SDK to use Unreal's memory management 
 * (FMemory through overloaded new/delete operators by default) for dynamic allocations
 */ 
class MemoryManagerWrapper : public Aws::Utils::Memory::MemorySystemInterface
{
public:
    void* AllocateMemory(
        std::size_t blockSize, std::size_t alignment,
        const char* allocationTag = nullptr) override;
    void FreeMemory(void* memoryPtr) override;
    void Begin() override {};
    void End() override {};
};

/**
 * Manages the runtime resources required by the AmazonPolly Plugin. In particular, this includes
 * loading and unloading of the AWS libraries during module startup and shutdown.
 *
 * @see IModuleInterface for details
 */
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

private:

    /**
     * Reference to Aws::SDKOptions. This is needed to make sure to pass the same
     * instance to Aws::InitAPI and Aws::ShutdownAPI as dictated by the
     * [AWS docs](). Using type `void *` to encapsulate implementation details of
     * this module (i.e. including the AWS SDK headers in this module's interface).
     */
    void* m_sdkOptions{ nullptr };

    bool m_apiInitialized{ false };

    MemoryManagerWrapper m_memoryManager;
};