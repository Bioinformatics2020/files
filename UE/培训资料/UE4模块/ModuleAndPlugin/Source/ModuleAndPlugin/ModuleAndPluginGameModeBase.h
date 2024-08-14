// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ModuleAndPluginGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MODULEANDPLUGIN_API AModuleAndPluginGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
};
