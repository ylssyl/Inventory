// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BagGridDragWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UBagGridDragWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		class UImage* ItemIconImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemNumText;

	UFUNCTION()
		void Init(int InGridIndex);
};
