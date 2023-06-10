// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "InventoryGM.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API AInventoryGM : public AGameMode
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
		void AddItem(int ID, int Amount, FVector Location);

	void UpdateGroundItem(int ID, FVector Location, FName Index);
	
};
