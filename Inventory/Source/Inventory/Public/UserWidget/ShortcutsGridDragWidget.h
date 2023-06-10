// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShortcutsGridDragWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UShortcutsGridDragWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		class UImage* ItemIconImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemNumText;

	UFUNCTION()
		void Init(int InShortcutsGridIndex);
};
