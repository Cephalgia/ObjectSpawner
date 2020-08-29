// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ObjectSpawner.h"
#include "ObjectSpawnerConfig.h"

#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#define LOCTEXT_NAMESPACE "FObjectSpawnerModule"

void FObjectSpawnerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create the new category
		ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		SettingsContainer->DescribeCategory("ObjectSpawner",
			LOCTEXT("RuntimeWDCategoryName", "Plugins"),
			LOCTEXT("RuntimeWDCategoryDescription", "Base configuration for ObjectSpawner"));

		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "ObjectSpawner Settings",
			LOCTEXT("RuntimeGeneralSettingsName", "ObjectSpawner Settings"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Base configuration for ObjectSpawner"),
			GetMutableDefault<UALSConfig>()
		);
	}
}

void FObjectSpawnerModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "Plugins", "ObjectSpawner Settings");
		}
	}
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FObjectSpawnerModule, ObjectSpawner)
