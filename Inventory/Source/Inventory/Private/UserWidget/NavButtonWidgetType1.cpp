// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/NavButtonWidgetType1.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/EventManager/EventManager.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"

void UNavButtonWidgetType1::Init(FNavButtonAttr NewNavButtonAttr)
{
	this->NavButtonAttr = NewNavButtonAttr;
	this->SetVisibility(ESlateVisibility::Collapsed);
	HotKeyText->SetText(FText::FromName(NavButtonAttr.HotKey));

	FScriptDelegate TestDelegate;//委托的申明
	TestDelegate.BindUFunction(this, FName("OnNavButtonClicked"));//委托的绑定
	NavButton->OnClicked.Add(TestDelegate);//点击委托绑定

	ButtonSizeBox->SetWidthOverride(NavButtonAttr.ButtonSize);
	ButtonSizeBox->SetHeightOverride(NavButtonAttr.ButtonSize);

	ADataAssetMananger::RequestAsyncLoadObjects
	(
		this, 
		{ NavButtonAttr.NormalIcon, NavButtonAttr.HoverIcon, NavButtonAttr.PressedIcon },
		[this](TArray<UObject*> Assets) 
		{
			FButtonStyle* ButtonStyle = &(NavButton->WidgetStyle);
			ButtonStyle->Normal.SetResourceObject(Assets[0]);
			ButtonStyle->Normal.DrawAs = ESlateBrushDrawType::Image;

			ButtonStyle->Hovered.SetResourceObject(Assets[1]);
			ButtonStyle->Hovered.DrawAs = ESlateBrushDrawType::Image;

			ButtonStyle->Pressed.SetResourceObject(Assets[2]);
			ButtonStyle->Pressed.DrawAs = ESlateBrushDrawType::Image;

			this->SetVisibility(ESlateVisibility::Visible);
		}
	);
}

void UNavButtonWidgetType1::OnNavButtonClicked()
{
	FEventManager::TriggerEvent(this->NavButtonAttr.ClickedEvent, &NavButtonAttr.OpenWidgetType);
}

bool UNavButtonWidgetType1::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return true;
}

