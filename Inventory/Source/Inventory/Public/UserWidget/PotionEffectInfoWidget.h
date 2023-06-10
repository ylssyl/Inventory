// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Object/CommonTypes.h"
#include "PotionEffectInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UPotionEffectInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PropertyNameText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AddAmountText;
	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* DurationPanel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DurationText;
	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* EffectDurationPanel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* EffectDurationText;

	UFUNCTION()
		void Init(FName PropertyName, FPropertyEffect PropertyEffect);
};
