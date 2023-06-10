// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Object/CommonTypes.h"
#include "HUDWidget.generated.h"

/**
 *
 */
UCLASS()
class INVENTORY_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()override;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* NavButtonHorizontalPanel;
	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* NavButtonVerticalPanel;
	UPROPERTY(meta = (BindWidget))
		class UNamedSlot* BagWidgetPanel;
	UPROPERTY(meta = (BindWidget))
		class UNamedSlot* ShortcutsWidgetPanel;
	UPROPERTY(meta = (BindWidget))
		class UNamedSlot* EquipmentWidgetPanel;

	void CreateNavButtons();

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	UFUNCTION(BlueprintCallable)
		void ToggleBagWidget(FName OpenWidgetType);
	UFUNCTION()
		void ToggleSettingWidget();
	UFUNCTION(BlueprintCallable)
		void ToggleEqumentWidget(FName OpenWidgetType);

	UFUNCTION()
		void CloseBag();
	void OpenBag(FName OpenWidgetType);

	UFUNCTION()
		void CloseEquipment();
	void OpenEquipment(FName OpenWidgetType);
	

private:
	TArray<FNavButtonAttr> NavButtonAttrs;
	bool IsBagOpened;
	UUserWidget* BagWidget;
	UUserWidget* EquipmentWidget;
};
