// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/EquipmentWidgetType1.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "Blueprint/WidgetTree.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Inventory/EventManager/EventManager.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/UserInterfaceSettings.h"

void UEquipmentWidgetType1::Init(FName EquipmentType)
{
	//UKismetSystemLibrary::PrintString(0, EquipmentType.ToString());
	FEquipmentWidgetType1Attr EquipmentWidgetType1Attr = FDataTableTool::GetEquipmentWidgetType1Attr(EquipmentType);
	//this->SetVisibility(ESlateVisibility::Collapsed);

	EquipmentSizeBox->SetWidthOverride(EquipmentWidgetType1Attr.BagSize.X);
	EquipmentSizeBox->SetHeightOverride(EquipmentWidgetType1Attr.BagSize.Y);
	UClass* EquipmentWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, EquipmentWidgetType1Attr.EquipmentGridType);

	for (int GridIndex = 0; GridIndex < 6; GridIndex++)
	{
		UUserWidget* EquipmentGridWidget = WidgetTree->ConstructWidget<UUserWidget>(EquipmentWidgetClass);
		FEquipmentGridType1Attr EquipmentGridType1Attr = FDataTableTool::GetEquipmentGridType1Attr(IntToName(GridIndex));
		check(EquipmentGridWidget);
		EquipmentGridPanel->AddChild(EquipmentGridWidget);
		UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(EquipmentGridWidget);
		CanvasPanelSlot->SetPosition(EquipmentGridType1Attr.Location);
		CanvasPanelSlot->SetAlignment(FVector2D(0, 0.5));
		CanvasPanelSlot->SetAutoSize(true);

		UFunction* InitFun = EquipmentGridWidget->FindFunction(FName("Init"));
		if (InitFun)
		{
			//UKismetSystemLibrary::PrintString(0, FString::FromInt(GridIndex));
			EquipmentGridWidget->ProcessEvent(InitFun, &GridIndex);
		}
	}

}

void UEquipmentWidgetType1::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEquipmentWidgetType1::NativeDestruct()
{
	Super::NativeDestruct();
}

FReply UEquipmentWidgetType1::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return FReply::Handled();
}

void UEquipmentWidgetType1::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UEquipmentWidgetType1::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return true;
}
