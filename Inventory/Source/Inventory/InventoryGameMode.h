// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InventoryGameMode.generated.h"

UCLASS(minimalapi)
class AInventoryGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInventoryGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void AddItem(int ID, int Amount, FVector Location);

	void UpdateGroundItem(int ID, FVector Location, FName Index);
};



