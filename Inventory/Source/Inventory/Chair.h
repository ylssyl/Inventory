// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Object/CommonTypes.h"
#include "Chair.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API AChair : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AChair();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void PrintGameSaveData(FGameSaveData GameSaveData);
	
};
