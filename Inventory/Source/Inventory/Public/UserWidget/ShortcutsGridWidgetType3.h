// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShortcutsGridWidgetType3.generated.h"

/**
 *
 */
UCLASS()
class INVENTORY_API UShortcutsGridWidgetType3 : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
		class USizeBox* ShortcutsGridSizeBox;

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

	UFUNCTION()
		void Init(int InGridIndex);
	int GetShortoutsGridIndex() { return ShortoutsGridIndex; }

	UFUNCTION()
		void UpdateDisPlay(int InGridIndex);
	UFUNCTION()
		void UpdateDisPlay2(int InGridIndex);

	UFUNCTION()
		void StartSelectShortcutsBar(int InShortcutsGridIndex);
	UFUNCTION()
		void EndSelectShortcutsBar(int InShortcutsGridIndex);
	UFUNCTION()
		void UseShortcutsItem(int InShortcutsGridIndex);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct()override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	void SetHalfOpacity(class UDragDropOperation* Operation);
	UFUNCTION()
		void SetFullOpacity(class UDragDropOperation* Operation);

private:
	int ShortoutsGridIndex;
};
