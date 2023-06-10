// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/BagGridWidgetType1.h"
#include "components/SizeBox.h"
#include "components/Border.h"
#include "components/Image.h"
#include "components/TextBlock.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Inventory/EventManager/EventManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Object/BagGridDDO.h"
#include "Object/ShortcutsBarGridDDO.h"
#include "Kismet/GameplayStatics.h"

void UBagGridWidgetType1::Init(int InGridIndex)
{
	this->GridIndex = InGridIndex;

	UpdateDisPlay(this->GridIndex);
}

void UBagGridWidgetType1::UpdateDisPlay(int InGridIndex)
{
	if (this->GridIndex != InGridIndex) return;

	UpdateDisPlay2();
}

void UBagGridWidgetType1::UpdateDisPlay2()
{
	//读取数据
	FBagGridData BagGridData = FFileTool::GetPlayerBagGridDataByIndex(GridIndex);

	FItemOnBagGridAttr ItemOnBagGridAttr = FDataTableTool::GetItemOnBagGridAttr(FName(*FString::FromInt(BagGridData.ID)));

	ItemNumText->SetVisibility((BagGridData.Num > 1) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (BagGridData.Num > 1)
	{
		ItemNumText->SetText(FText::FromString(FString::FromInt(BagGridData.Num)));
		//ItemOnBagGridAttr.MaxStackNum== BagGridData.Num
		ItemNumText->SetColorAndOpacity((ItemOnBagGridAttr.MaxStackNum == BagGridData.Num) ? FLinearColor::Red : FLinearColor::White);
	}

	GridSizeBox->SetWidthOverride(ItemOnBagGridAttr.Size.X);
	GridSizeBox->SetHeightOverride(ItemOnBagGridAttr.Size.Y);

	ItemImage->SetVisibility(BagGridData.ID != 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	ADataAssetMananger::RequestAsyncLoadObjects(
		this,
		{ ItemOnBagGridAttr.BGIcon,ItemOnBagGridAttr.BorderIcon },
		[this](TArray<UObject*> Assets) {
			BgBorder->SetBrushFromTexture(Cast<UTexture2D>(Assets[0]));
			ItemBorder->SetBrushFromTexture(Cast<UTexture2D>(Assets[1]));
		}
	);

	if (BagGridData.ID != 0)
	{
		ADataAssetMananger::RequestAsyncLoadObject(
			this,
			ItemOnBagGridAttr.ItemIcon,
			[this](UObject* Asset) {
				ItemImage->SetBrushFromTexture(Cast<UTexture2D>(Asset));
			}
		);

		DotImage->SetVisibility(FFileTool::IsHadItem(BagGridData.ID) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	}
}

void UBagGridWidgetType1::PlayZoomAnim(int InGridIndex)
{
	if (this->GridIndex != InGridIndex) return;

	this->PlayAnimation(ZoomAnim);
}

void UBagGridWidgetType1::OnStartCheckBagItem(int InGridIndex)
{
	if (FFileTool::GetPlayerBagGridDataByIndex(InGridIndex).ID == FFileTool::GetPlayerBagGridDataByIndex(this->GridIndex).ID)
	{
		DotImage->SetVisibility(
			FFileTool::IsHadItem(FFileTool::GetPlayerBagGridDataByIndex(InGridIndex).ID)
			?
			ESlateVisibility::Collapsed :
			ESlateVisibility::SelfHitTestInvisible
		);
	}


}

void UBagGridWidgetType1::NativeConstruct()
{
	Super::NativeConstruct();
	FEventManager::RegistEvent(FName("BagAddItemEvent"), this, FName("UpdateDisPlay"));
	FEventManager::RegistEvent(FName("BagGridDragToBagGridFinishedEvent"), this, FName("UpdateDisplay"));
	FEventManager::RegistEvent(FName("ClearBagGridByIndexFinishedEvent"), this, FName("UpdateDisplay"));
	FEventManager::RegistEvent(FName("BagAddItemEvent"), this, FName("PlayZoomAnim"));
	FEventManager::RegistEvent(FName("HadCheckBagItemEvent"), this, FName("OnStartCheckBagItem"));
	FEventManager::RegistEvent(FName("BagItemSortFinishedEvent"), this, FName("UpdateDisPlay2"));
	FEventManager::RegistEvent(FName("BagOfBagGridDragToShortoutsGridFinishedEvent"), this, FName("UpdateDisplay"));
	FEventManager::RegistEvent(FName("BagOfShortcutsGridDragToBagGridFinishedEvent"), this, FName("UpdateDisplay"));
}

void UBagGridWidgetType1::NativeDestruct()
{
	Super::NativeDestruct();
	FEventManager::UnRegistEvent(FName("BagAddItemEvent"), this);
	FEventManager::UnRegistEvent(FName("BagGridDragToBagGridFinishedEvent"), this);
	FEventManager::UnRegistEvent(FName("ClearBagGridByIndexFinishedEvent"), this);
	FEventManager::UnRegistEvent(FName("BagAddItemEvent"), this);
	FEventManager::UnRegistEvent(FName("HadCheckBagItemEvent"), this);
	FEventManager::UnRegistEvent(FName("BagItemSortFinishedEvent"), this);
	FEventManager::UnRegistEvent(FName("BagOfBagGridDragToShortoutsGridFinishedEvent"), this);
	FEventManager::UnRegistEvent(FName("BagOfShortcutsGridDragToBagGridFinishedEvent"), this);
}

void UBagGridWidgetType1::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	FBagGridData BagGridData = FFileTool::GetPlayerBagGridDataByIndex(GridIndex);
	if (BagGridData.ID != 0)
	{
		SelectImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FEventManager::TriggerEvent(FName("StartCheckBagGrid"), &GridIndex);

		FItemOnBagGridAttr ItemOnBagGridAttr = FDataTableTool::GetItemOnBagGridAttr(IntToName(BagGridData.ID));

		UClass* TipBorderWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemOnBagGridAttr.TipBorderWidgetClass);

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
			Prams.ID = BagGridData.ID;
			Prams.Amount = BagGridData.Num;
			Prams.TipContextWidgetClass = ItemOnBagGridAttr.TipContextWidgetClass;
			TipBorderWidget->ProcessEvent(InitFun, &Prams);
		}

	}
}

void UBagGridWidgetType1::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	FBagGridData BagGridData = FFileTool::GetPlayerBagGridDataByIndex(GridIndex);
	if (BagGridData.ID != 0)
	{
		SelectImage->SetVisibility(ESlateVisibility::Collapsed);
		FEventManager::TriggerEvent(FName("EndCheckBagGrid"), &GridIndex);
	}
}

FReply UBagGridWidgetType1::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && FFileTool::GetPlayerBagGridDataByIndex(GridIndex).ID != 0)
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton);

		return EventReply.NativeReply;
	}

	return Reply;
}

void UBagGridWidgetType1::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UBagGridDDO* DDO = Cast<UBagGridDDO>(UWidgetBlueprintLibrary::CreateDragDropOperation(UBagGridDDO::StaticClass()));
	DDO->FromGridIndex = this->GridIndex;

	FItemOnBagGridAttr ItemOnBagGridAttr = FDataTableTool::GetItemOnBagGridAttr(IntToName(FFileTool::GetPlayerBagGridDataByIndex(GridIndex).ID));

	UClass* BagGridDragWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemOnBagGridAttr.DragWidgetClass);

	UUserWidget* BagGridDragWidget = CreateWidget<UUserWidget>(this->GetOwningPlayer(), BagGridDragWidgetClass);

	UFunction* FunPtr = BagGridDragWidget->FindFunction("Init");
	if (FunPtr)
	{
		BagGridDragWidget->ProcessEvent(FunPtr, &GridIndex);
	}

	DDO->DefaultDragVisual = BagGridDragWidget;
	DDO->Pivot = EDragPivot::MouseDown;
	OutOperation = DDO;
	DDO->OnDrop.AddDynamic(this, &UBagGridWidgetType1::SetFullOpacity);
	DDO->OnDragCancelled.AddDynamic(this, &UBagGridWidgetType1::SetFullOpacity);
	SetHalfOpacity(DDO);
}

bool UBagGridWidgetType1::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (InOperation->IsA<UBagGridDDO>())
	{
		UBagGridDDO* BagGridDDO = Cast<UBagGridDDO>(InOperation);
		struct
		{
			int FromIndex;
			int ToIndex;
		}Params;
		Params.FromIndex = BagGridDDO->FromGridIndex;
		Params.ToIndex = this->GridIndex;
		FEventManager::TriggerEvent(FName("BagGridDragToBagGridEvent"), &Params);
	}

	if (InOperation->IsA<UShortcutsBarGridDDO>())
	{
		UShortcutsBarGridDDO* ShortcutsBarGridDDO = Cast<UShortcutsBarGridDDO>(InOperation);
		struct
		{
			int FromIndex;
			int ToIndex;
		}Params;
		Params.FromIndex = ShortcutsBarGridDDO->FromGridIndex;
		Params.ToIndex = this->GridIndex;
		FEventManager::TriggerEvent(FName("BagOfShortcutsGridDragToBagGridEvent"), &Params);
	}

	return true;
}

FReply UBagGridWidgetType1::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	FBagGridData BagGridData = FFileTool::GetPlayerBagGridDataByIndex(GridIndex);
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && BagGridData.ID != 0)
	{
		FItemCommonAttr ItemCommonAttr = FDataTableTool::GetItemCommonAttr(IntToName(BagGridData.ID));
		if (ItemCommonAttr.UsedItem == FName("EquipmentActor"))
		{
			UClass* EquipmentActorClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemCommonAttr.UsedItem);
			//EquipmentActorClass->FindFunctionByName(FName());
			AActor* DefaultEquipmentActor = Cast<AActor>(EquipmentActorClass->GetDefaultObject());//反射
			UFunction* EquipFunc = DefaultEquipmentActor->FindFunction(FName("EquipItemFromBagGrid"));
			if (EquipFunc)
			{
				struct
				{
					int BagIndex;
					int EquipId;
				}Params;
				Params.BagIndex = GridIndex;
				Params.EquipId = BagGridData.ID;
				DefaultEquipmentActor->ProcessEvent(EquipFunc, &Params);
				SelectImage->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}

	return FReply::Handled();
}

void UBagGridWidgetType1::SetHalfOpacity(UDragDropOperation* Operation)
{
	ItemImage->SetOpacity(0.5f);
	ItemNumText->SetOpacity(0.5f);
}

void UBagGridWidgetType1::SetFullOpacity(UDragDropOperation* Operation)
{
	ItemImage->SetOpacity(1.0f);
	ItemNumText->SetOpacity(1.0f);
}


