// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/EquipmentGridWidgetType1.h"
#include "components/SizeBox.h"
#include "components/Border.h"
#include "components/Image.h"
#include "components/TextBlock.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Inventory/EventManager/EventManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Object/BagGridDDO.h"


void UEquipmentGridWidgetType1::Init(int InIndex)
{
	EquipmentIndex = InIndex;
	UpdateDisPlay(InIndex);
}

void UEquipmentGridWidgetType1::UpdateDisPlay(int InGridIndex)
{
	if (this->EquipmentIndex != InGridIndex) return;
	//读取数据
	FEquipmentGridData EquipmentGridData = FFileTool::GetEquipmentGridDataByIndex(InGridIndex);
	FItemOnEquipmentGridAttr ItemOnEquipmentGridAttr = FDataTableTool::GetItemOnEquipmentGridAttr(FName(*FString::FromInt(EquipmentGridData.EquipmentGridDataID)));

	ItemNumText->SetVisibility((EquipmentGridData.EquipmentGridDataNum > 1) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (EquipmentGridData.EquipmentGridDataNum > 1)
	{
		ItemNumText->SetText(FText::FromString(FString::FromInt(EquipmentGridData.EquipmentGridDataNum)));
		ItemNumText->SetColorAndOpacity((ItemOnEquipmentGridAttr.MaxStackNum == EquipmentGridData.EquipmentGridDataNum) ? FLinearColor::Red : FLinearColor::White);
	}

	EquipmentGridSizeBox->SetWidthOverride(ItemOnEquipmentGridAttr.Size.X);
	EquipmentGridSizeBox->SetHeightOverride(ItemOnEquipmentGridAttr.Size.Y);

	ItemImage->SetVisibility(EquipmentGridData.EquipmentGridDataID != 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	ADataAssetMananger::RequestAsyncLoadObjects(
		this,
		{ ItemOnEquipmentGridAttr.BGIcon,ItemOnEquipmentGridAttr.BorderIcon },
		[this](TArray<UObject*> Assets) {
			BgBorder->SetBrushFromTexture(Cast<UTexture2D>(Assets[0]));
			ItemBorder->SetBrushFromTexture(Cast<UTexture2D>(Assets[1]));
		}
	);

	if (EquipmentGridData.EquipmentGridDataID != 0)
	{
		ADataAssetMananger::RequestAsyncLoadObject(
			this,
			ItemOnEquipmentGridAttr.ItemIcon,
			[this](UObject* Asset) {
				ItemImage->SetBrushFromTexture(Cast<UTexture2D>(Asset));
			}
		);


	}
	struct
	{
		int Id;
		int Index;
	}Params;
	Params.Id = EquipmentGridData.EquipmentGridDataID;
	Params.Index = EquipmentIndex;
	FEventManager::TriggerEvent(FName("EquipItemLoad1Event"), &Params);

}

void UEquipmentGridWidgetType1::ClearEquipmentGridByIndex(int InGridIndex)
{
	if (InGridIndex != EquipmentIndex) return;

	FFileTool::ClearPlayerEquipmentGridByIndex(InGridIndex);
}

void UEquipmentGridWidgetType1::NativeConstruct()
{
	Super::NativeConstruct();
	FEventManager::RegistEvent(FName("EquipItemFinishedEvent"), this, FName("UpdateDisPlay"));
}

void UEquipmentGridWidgetType1::NativeDestruct()
{
	Super::NativeDestruct();
	FEventManager::UnRegistEvent(FName("EquipItemFinishedEvent"), this);
}

void UEquipmentGridWidgetType1::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	FEquipmentGridData EquipmentGridData = FFileTool::GetEquipmentGridDataByIndex(EquipmentIndex);
	if (EquipmentGridData.EquipmentGridDataID != 0)
	{
		SelectImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FItemOnEquipmentGridAttr ItemOnEquipmentGridAttr = FDataTableTool::GetItemOnEquipmentGridAttr(IntToName(EquipmentGridData.EquipmentGridDataID));

		UClass* TipBorderWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemOnEquipmentGridAttr.TipBorderWidgetClass);

		UUserWidget* TipBorderWidget = CreateWidget<UUserWidget>(this->GetOwningPlayer(), TipBorderWidgetClass);
		ItemImage->SetToolTip(TipBorderWidget);

		UFunction* InitFun = TipBorderWidget->FindFunction(FName("Init"));
		if (InitFun)
		{
			struct
			{
				int ID;
				int Amount;
				FName TipContextWidgetClass;
			}Prams;
			Prams.ID = EquipmentGridData.EquipmentGridDataID;
			Prams.Amount = EquipmentGridData.EquipmentGridDataNum;
			Prams.TipContextWidgetClass = ItemOnEquipmentGridAttr.TipContextWidgetClass;
			TipBorderWidget->ProcessEvent(InitFun, &Prams);
		}

	}
}

void UEquipmentGridWidgetType1::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	FEquipmentGridData EquipmentGridData = FFileTool::GetEquipmentGridDataByIndex(EquipmentIndex);
	if (EquipmentGridData.EquipmentGridDataID != 0)
	{
		SelectImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FReply UEquipmentGridWidgetType1::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && FFileTool::GetEquipmentGridDataByIndex(EquipmentIndex).EquipmentGridDataID != 0)
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		FEquipmentGridData EquipmentGridData = FFileTool::GetEquipmentGridDataByIndex(EquipmentIndex);
		ClearEquipmentGridByIndex(EquipmentIndex);
		UpdateDisPlay(EquipmentIndex);
		struct
		{
			int Id;
			int Num;
		}Prams;
		Prams.Id = EquipmentGridData.EquipmentGridDataID;
		Prams.Num = EquipmentGridData.EquipmentGridDataNum;
		
		FEventManager::TriggerEvent(FName("aaa"), &Prams);//
		SelectImage->SetVisibility(ESlateVisibility::Collapsed);
		UpdateDisPlay(EquipmentIndex);
		return EventReply.NativeReply;
	}

	return Reply;
}

void UEquipmentGridWidgetType1::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UEquipmentGridWidgetType1::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (InOperation->IsA<UBagGridDDO>())
	{
		UBagGridDDO* BagGridDDO = Cast<UBagGridDDO>(InOperation);
		FBagGridData BagGridData = FFileTool::GetPlayerBagGridDataByIndex(BagGridDDO->FromGridIndex);
		FItemCommonAttr ItemCommonAttr = FDataTableTool::GetItemCommonAttr(IntToName(BagGridData.ID));



		if (ItemCommonAttr.UsedItem == FName("EquipmentActor"))
		{
			struct
			{
				int FromIndex;
				int ToIndex;
			}Params;
			Params.FromIndex = BagGridDDO->FromGridIndex;
			Params.ToIndex = this->EquipmentIndex;
			FEventManager::TriggerEvent(FName("EquipItemEvent"), &Params);//先清除背包格子的数据，在添加到装备栏
		}
	}

	return true;
}
