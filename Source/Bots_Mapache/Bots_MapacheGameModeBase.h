// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Bots_MapacheGameModeBase.generated.h"

/**
 * GameMode base. Solo define las clases por defecto; la logica del mundo va en subsistemas.
 */
UCLASS()
class BOTS_MAPACHE_API ABots_MapacheGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABots_MapacheGameModeBase();
};
