// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Object/CommonTypes.h"
#include "NavButtonWidgetType1.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UNavButtonWidgetType1 : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class USizeBox* ButtonSizeBox;
	UPROPERTY(meta = (BindWidget))
		class UButton* NavButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HotKeyText;


	UFUNCTION()
		void Init(FNavButtonAttr NavButtonAttr);
	UFUNCTION()
		void OnNavButtonClicked();

	//virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;


private:
	FNavButtonAttr NavButtonAttr;
	
};
