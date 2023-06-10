// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TipContextWidgetType1.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UTipContextWidgetType1 : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* PropertyBox;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DesText;

	UFUNCTION()
		void Init(int ID);
};
