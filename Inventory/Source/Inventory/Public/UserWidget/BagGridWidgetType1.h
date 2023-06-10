// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BagGridWidgetType1.generated.h"

/**
 *
 */
UCLASS()
class INVENTORY_API UBagGridWidgetType1 : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
		class USizeBox* GridSizeBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
		class UBorder* BgBorder;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
		class UImage* SelectImage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
		class UBorder* ItemBorder;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
		class UImage* ItemImage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
		class UTextBlock* ItemNumText;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
		class UImage* DotImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* ZoomAnim;

	UFUNCTION()
		void Init(int InGridIndex);
	int GetGridIndex() { return GridIndex; }

	UFUNCTION()
		void UpdateDisPlay(int InGridIndex);
	UFUNCTION()
		void UpdateDisPlay2();
	UFUNCTION()
		void PlayZoomAnim(int InGridIndex);
	UFUNCTION()
		void OnStartCheckBagItem(int InGridIndex);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct()override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	void SetHalfOpacity(class UDragDropOperation* Operation);
	UFUNCTION()
		void SetFullOpacity(class UDragDropOperation* Operation);
private:
	int GridIndex;
};
