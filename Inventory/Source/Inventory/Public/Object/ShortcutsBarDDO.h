// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ShortcutsBarDDO.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UShortcutsBarDDO : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	FVector2D DragOffset;

	FGeometry DragGeometry;
};
