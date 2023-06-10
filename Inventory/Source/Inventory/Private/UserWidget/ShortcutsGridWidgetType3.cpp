// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/ShortcutsGridWidgetType3.h"
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
#include "Object/ShortcutsBarGridDDO.h"
#include "Object/BagGridDDO.h"

void UShortcutsGridWidgetType3::Init(int InGridIndex)
{
	this->ShortoutsGridIndex = InGridIndex;


	UpdateDisPlay(this->ShortoutsGridIndex);
}

void UShortcutsGridWidgetType3::UpdateDisPlay(int InGridIndex)
{
	if (this->ShortoutsGridIndex != InGridIndex) return;

	UpdateDisPlay2(InGridIndex);
}

void UShortcutsGridWidgetType3::UpdateDisPlay2(int InShortcutsGridIndex)
{
	//FShortcutsGridData InShortcutsGridData(10011, 1);
	//FFileTool::SetShortcutsGridDataByIndex(InGridIndex, InShortcutsGridData);
	//¶ÁÈ¡Êý¾Ý
	FShortcutsGridData ShortcutsGridData = FFileTool::GetShortcutsGridDataByIndex(InShortcutsGridIndex);

	//UKismetSystemLibrary::PrintString(0, "aaaa" + FString::FromInt(ShortcutsGridData.ShortcutsGridDataID));


	FItemOnShortcutsGridAttr ItemOnShortcutsGridAttr = FDataTableTool::GetItemOnShortcutsGridAttr(FName(*FString::FromInt(ShortcutsGridData.ShortcutsGridDataID)));

	ItemNumText->SetVisibility((ShortcutsGridData.ShortcutsGridDataNum > 1) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (ShortcutsGridData.ShortcutsGridDataNum > 1)
	{
		ItemNumText->SetText(FText::FromString(FString::FromInt(ShortcutsGridData.ShortcutsGridDataNum)));
		ItemNumText->SetColorAndOpacity((ItemOnShortcutsGridAttr.MaxStackNum == ShortcutsGridData.ShortcutsGridDataNum) ? FLinearColor::Red : FLinearColor::White);
	}

	ShortcutsGridSizeBox->SetWidthOverride(ItemOnShortcutsGridAttr.Size.X);
	ShortcutsGridSizeBox->SetHeightOverride(ItemOnShortcutsGridAttr.Size.Y);

	ItemImage->SetVisibility(ShortcutsGridData.ShortcutsGridDataID != 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	ADataAssetMananger::RequestAsyncLoadObjects(
		this,
		{ ItemOnShortcutsGridAttr.BGIcon,ItemOnShortcutsGridAttr.BorderIcon },
		[this](TArray<UObject*> Assets) {
			BgBorder->SetBrushFromTexture(Cast<UTexture2D>(Assets[0]));
			ItemBorder->SetBrushFromTexture(Cast<UTexture2D>(Assets[1]));
		}
	);

	if (ShortcutsGridData.ShortcutsGridDataID != 0)
	{
		ADataAssetMananger::RequestAsyncLoadObject(
			this,
			ItemOnShortcutsGridAttr.ItemIcon,
			[this](UObject* Asset) {
				ItemImage->SetBrushFromTexture(Cast<UTexture2D>(Asset));
			}
		);

	}
}

void UShortcutsGridWidgetType3::StartSelectShortcutsBar(int InShortcutsGridIndex)
{
	if (InShortcutsGridIndex == this->ShortoutsGridIndex)
	{
		SelectImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

}

void UShortcutsGridWidgetType3::EndSelectShortcutsBar(int InShortcutsGridIndex)
{
	if (InShortcutsGridIndex == this->ShortoutsGridIndex)
	{
		SelectImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UShortcutsGridWidgetType3::UseShortcutsItem(int InShortcutsGridIndex)
{
	if (InShortcutsGridIndex == this->ShortoutsGridIndex)
	{
		FShortcutsGridData ShortcutsGridData = FFileTool::GetShortcutsGridDataByIndex(InShortcutsGridIndex);
		if (ShortcutsGridData.ShortcutsGridDataID == 0) return;

		FItemCommonAttr ItemCommonAttr = FDataTableTool::GetItemCommonAttr(IntToName(ShortcutsGridData.ShortcutsGridDataID));
		if (ShortcutsGridData.ShortcutsGridDataNum == 1)
		{
			//FFileTool::SetShortcutsGridDataByIndex(ShortoutsGridIndex, FShortcutsGridData(0, 0));
			UKismetSystemLibrary::PrintString(0, FString("Num1"));
		}
		else
		{
			//ShortcutsGridData.ShortcutsGridDataNum -= 1;
			//FFileTool::SetShortcutsGridDataByIndex(ShortoutsGridIndex, ShortcutsGridData);
			UKismetSystemLibrary::PrintString(0, FString("Num2"));
		}
		UpdateDisPlay2(ShortoutsGridIndex);
		
	}
}

void UShortcutsGridWidgetType3::NativeConstruct()
{
	Super::NativeConstruct();
	FEventManager::RegistEvent(FName("StartSelectShortcutsBar"), this, FName("StartSelectShortcutsBar"));
	FEventManager::RegistEvent(FName("EndSelectShortcutsBar"), this, FName("EndSelectShortcutsBar"));
	FEventManager::RegistEvent(FName("ShortcutsGridDragToShortcutsGridFinishedEvent"), this, FName("UpdateDisPlay"));
	FEventManager::RegistEvent(FName("ClearShortcutsGridByIndexFinishedEvent"), this, FName("UpdateDisPlay"));
	FEventManager::RegistEvent(FName("ShortcutsOfShortcutsGridDragToBagGridFinishedEvent"), this, FName("UpdateDisPlay"));
	FEventManager::RegistEvent(FName("ShortoutsOfBagGridDragToShortoutsGridFinishedEvent"), this, FName("UpdateDisPlay"));
	FEventManager::RegistEvent(FName("StartUseShortcutsItemEvent"), this, FName("UseShortcutsItem"));
}

void UShortcutsGridWidgetType3::NativeDestruct()
{
	Super::NativeDestruct();
	FEventManager::UnRegistEvent(FName("StartSelectShortcutsBar"), this);
	FEventManager::UnRegistEvent(FName("EndSelectShortcutsBar"), this);
	FEventManager::UnRegistEvent(FName("ShortcutsGridDragToShortcutsGridFinishedEvent"), this);
	FEventManager::UnRegistEvent(FName("ClearShortcutsGridByIndexFinishedEvent"), this);
	FEventManager::UnRegistEvent(FName("ShortcutsOfShortcutsGridDragToBagGridFinishedEvent"), this);
	FEventManager::UnRegistEvent(FName("ShortoutsOfBagGridDragToShortoutsGridFinishedEvent"), this);
	FEventManager::UnRegistEvent(FName("StartUseShortcutsItemEvent"), this);
}

void UShortcutsGridWidgetType3::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	FShortcutsGridData ShortcutsGridData = FFileTool::GetShortcutsGridDataByIndex(ShortoutsGridIndex);
	if (ShortcutsGridData.ShortcutsGridDataID != 0)
	{
		SelectImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		//FShortcutsGridData ShortcutsGridData = FFileTool::GetShortcutsGridDataByIndex(InShortcutsGridIndex);


		FItemOnShortcutsGridAttr ItemOnShortcutsGridAttr = FDataTableTool::GetItemOnShortcutsGridAttr(IntToName(ShortcutsGridData.ShortcutsGridDataID));

		UClass* TipBorderWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemOnShortcutsGridAttr.TipBorderWidgetClass);

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
			Prams.ID = ShortcutsGridData.ShortcutsGridDataID;
			Prams.Amount = ShortcutsGridData.ShortcutsGridDataNum;
			Prams.TipContextWidgetClass = ItemOnShortcutsGridAttr.TipContextWidgetClass;
			TipBorderWidget->ProcessEvent(InitFun, &Prams);
		}

	}
}

void UShortcutsGridWidgetType3::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	FShortcutsGridData ShortcutsGridData = FFileTool::GetShortcutsGridDataByIndex(ShortoutsGridIndex);
	if (ShortcutsGridData.ShortcutsGridDataID != 0)
	{
		SelectImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FReply UShortcutsGridWidgetType3::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && FFileTool::GetShortcutsGridDataByIndex(ShortoutsGridIndex).ShortcutsGridDataID != 0)
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton);

		return EventReply.NativeReply;
	}

	return Reply;
}

void UShortcutsGridWidgetType3::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UShortcutsBarGridDDO* DDO = Cast<UShortcutsBarGridDDO>(UWidgetBlueprintLibrary::CreateDragDropOperation(UShortcutsBarGridDDO::StaticClass()));
	DDO->FromGridIndex = this->ShortoutsGridIndex;
	FItemOnShortcutsGridAttr ItemOnShortcutsGridAttr = FDataTableTool::GetItemOnShortcutsGridAttr(IntToName(FFileTool::GetShortcutsGridDataByIndex(ShortoutsGridIndex).ShortcutsGridDataID));

	UClass* ShortcutsGridDragWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemOnShortcutsGridAttr.DragWidgetClass);

	UUserWidget* ShortcutsGridDragWidget = CreateWidget<UUserWidget>(this->GetOwningPlayer(), ShortcutsGridDragWidgetClass);

	UFunction* FunPtr = ShortcutsGridDragWidget->FindFunction("Init");
	if (FunPtr)
	{
		ShortcutsGridDragWidget->ProcessEvent(FunPtr, &ShortoutsGridIndex);
	}

	DDO->DefaultDragVisual = ShortcutsGridDragWidget;
	DDO->Pivot = EDragPivot::MouseDown;
	OutOperation = DDO;
	DDO->OnDrop.AddDynamic(this, &UShortcutsGridWidgetType3::SetFullOpacity);
	DDO->OnDragCancelled.AddDynamic(this, &UShortcutsGridWidgetType3::SetFullOpacity);
	SetHalfOpacity(DDO);
}

bool UShortcutsGridWidgetType3::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (InOperation->IsA<UShortcutsBarGridDDO>())
	{
		UShortcutsBarGridDDO* ShortcutsBarGridDDO = Cast<UShortcutsBarGridDDO>(InOperation);
		struct
		{
			int FromIndex;
			int ToIndex;
		}Params;
		Params.FromIndex = ShortcutsBarGridDDO->FromGridIndex;
		Params.ToIndex = this->ShortoutsGridIndex;
		FEventManager::TriggerEvent(FName("ShortcutsGridDragToShortcutsGridEvent"), &Params);
	}

	if (InOperation->IsA<UBagGridDDO>())
	{
		UBagGridDDO* BagGridDDO = Cast<UBagGridDDO>(InOperation);
		FBagGridData BagGridData = FFileTool::GetPlayerBagGridDataByIndex(BagGridDDO->FromGridIndex);
		FItemCommonAttr ItemCommonAttr = FDataTableTool::GetItemCommonAttr(IntToName(BagGridData.ID));
		if (ItemCommonAttr.UsedItem != FName("EquipmentActor"))
		{
			struct
			{
				int FromIndex;
				int ToIndex;
			}Params;
			Params.FromIndex = BagGridDDO->FromGridIndex;
			Params.ToIndex = this->ShortoutsGridIndex;
			FEventManager::TriggerEvent(FName("ShortcutsOfBagGridDragToShortcutsGridEvent"), &Params);
		}
	}

	return true;
}

void UShortcutsGridWidgetType3::SetHalfOpacity(UDragDropOperation* Operation)
{
	ItemImage->SetOpacity(0.5f);
	ItemNumText->SetOpacity(0.5f);
}

void UShortcutsGridWidgetType3::SetFullOpacity(UDragDropOperation* Operation)
{
	ItemImage->SetOpacity(1.0f);
	ItemNumText->SetOpacity(1.0f);
}
