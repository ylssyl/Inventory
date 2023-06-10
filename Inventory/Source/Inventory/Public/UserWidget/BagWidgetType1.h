// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Object/CommonTypes.h"
#include "BagWidgetType1.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UBagWidgetType1 : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
		class USizeBox* BagSizeBox;
	UPROPERTY(meta = (BindWidget))
		class UBorder* BgBorder;
	UPROPERTY(meta = (BindWidget))
		class UButton* CloseButton;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* GridsPanel;
	UPROPERTY(meta = (BindWidget))
		class UButton* SortItemButton;

	UFUNCTION()
		void Init(FName BagType);
	UFUNCTION()
		void OnCloseBagWidget();
	UFUNCTION()
		void OnSortItem();

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	UFUNCTION()
		void OnBagWidgetDragCancelled(class UDragDropOperation* Operation);

	class UPanelWidget* ParentPanelWidget;

};
