// Copyright Epic Games, Inc. All Rights Reserved.


#include "ModuleAndPluginGameModeBase.h"

#include "ModuleDemo/ModuleDemoFun.h"


void AModuleAndPluginGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	FModuleDemoFun a;
	a.Test();

	FModuleManager::Get().LoadModule("ModuleDemo2");

	fun();
}
