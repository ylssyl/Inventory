// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "BagGridDDO.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UBagGridDDO : public UDragDropOperation
{
	GENERATED_BODY()

public:
	int FromGridIndex;

	UFUNCTION()
		void OnDragFinish();
	
};
