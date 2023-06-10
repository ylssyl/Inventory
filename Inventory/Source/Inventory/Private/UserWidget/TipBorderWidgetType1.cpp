// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/TipBorderWidgetType1.h"
#include "Components/TextBlock.h"
#include "Components/NamedSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Library/DataTableTool.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"

void UTipBorderWidgetType1::Init(int ID, int Amount, FName TipContextWidgetClass)
{
	FItemCommonAttr ItemCommonAttr = FDataTableTool::GetItemCommonAttr(IntToName(ID));
	ItemNameText->SetText(FText::FromName(ItemCommonAttr.ItemName));
	NumText->SetText(FText::FromString(FString::FromInt(Amount)));

	UClass* TipContextWidgetType = ADataAssetMananger::RequestSyncLoadClass(this, TipContextWidgetClass);

	UUserWidget* TipContextWidget = CreateWidget<UUserWidget>(this->GetOwningPlayer(), TipContextWidgetType);
	ContextWidgetSlot->AddChild(TipContextWidget);

	UFunction* InitFun = TipContextWidget->FindFunction(FName("Init"));
	if (InitFun)
	{
		TipContextWidget->ProcessEvent(InitFun, &ID);
	}
}
