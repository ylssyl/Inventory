// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TipBorderWidgetType1.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UTipBorderWidgetType1 : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemNameText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* NumText;
	UPROPERTY(meta = (BindWidget))
		class UNamedSlot* ContextWidgetSlot;

	UFUNCTION()
		void Init(int ID, int Amount, FName TipContextWidgetClass);
};
