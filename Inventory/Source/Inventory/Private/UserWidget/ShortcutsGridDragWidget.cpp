// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/ShortcutsGridDragWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"

void UShortcutsGridDragWidget::Init(int InShortcutsGridIndex)
{
	FShortcutsGridData ShortcutsGridData = FFileTool::GetShortcutsGridDataByIndex(InShortcutsGridIndex);
	FItemOnShortcutsGridAttr ItemOnShortcutsGridAttr = FDataTableTool::GetItemOnShortcutsGridAttr(IntToName(ShortcutsGridData.ShortcutsGridDataID));

	ItemNumText->SetVisibility(ShortcutsGridData.ShortcutsGridDataNum <= 1 ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	ItemNumText->SetText(FText::FromString(FString::FromInt(ShortcutsGridData.ShortcutsGridDataNum)));
	ItemNumText->SetColorAndOpacity(ShortcutsGridData.ShortcutsGridDataNum == ItemOnShortcutsGridAttr.MaxStackNum ? FLinearColor::Red : FLinearColor::White);

	ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
	ADataAssetMananger::RequestAsyncLoadObject(this, ItemOnShortcutsGridAttr.ItemIcon,
		[this](UObject* Asset) {
			ItemIconImage->SetBrushFromTexture(Cast<UTexture2D>(Asset));
			ItemIconImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	);
}
