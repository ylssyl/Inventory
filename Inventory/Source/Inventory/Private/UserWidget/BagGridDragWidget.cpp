// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/BagGridDragWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"

void UBagGridDragWidget::Init(int InGridIndex)
{
	FBagGridData BagGridData = FFileTool::GetPlayerBagGridDataByIndex(InGridIndex);
	FItemOnBagGridAttr ItemOnBagGridAttr = FDataTableTool::GetItemOnBagGridAttr(IntToName(BagGridData.ID));

	ItemNumText->SetVisibility(BagGridData.Num <= 1 ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	ItemNumText->SetText(FText::FromString(FString::FromInt(BagGridData.Num)));
	ItemNumText->SetColorAndOpacity(BagGridData.Num == ItemOnBagGridAttr.MaxStackNum ? FLinearColor::Red : FLinearColor::White);

	ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
	ADataAssetMananger::RequestAsyncLoadObject(this, ItemOnBagGridAttr.ItemIcon,
		[this](UObject* Asset) {
			ItemIconImage->SetBrushFromTexture(Cast<UTexture2D>(Asset));
			ItemIconImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	);
}
