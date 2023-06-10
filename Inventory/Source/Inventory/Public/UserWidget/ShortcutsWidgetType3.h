// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShortcutsWidgetType3.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UShortcutsWidgetType3 : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* ShortcutsHorizontalBox;

	UFUNCTION()
		void Init(FName ShortcutsType);
};
