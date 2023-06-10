// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/HUDWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/NamedSlot.h"


#include "Kismet/GameplayStatics.h"
#include "Library/DataTableTool.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Inventory/EventManager/EventManager.h"
#include "Blueprint/WidgetTree.h"
#include "Object/BagGridDDO.h"
#include "Object/BagDDO.h"
#include "Object/ShortcutsBarGridDDO.h"

void UHUDWidget::NativeConstruct()
{
	CreateNavButtons();

	
	Super::NativeConstruct();//子类后调用
}

void UHUDWidget::NativeDestruct()
{
	//注销事件
	for (FNavButtonAttr NavButtonAttr : NavButtonAttrs)
	{
		FEventManager::UnRegistEvent(NavButtonAttr.ClickedEvent, this);
	}

	

	Super::NativeDestruct();
}

void UHUDWidget::CreateNavButtons()
{
	//UProperty* Property = this->GetClass()->FindPropertyByName(FName("NavButtonPanel"));
	//if (Property)
	//{//用反射来获取蓝图的widget组件(meta = (BindWidget)的原理)
	//	UObjectProperty* ObjectProperty = Cast<UObjectProperty>(Property);
	//	NavButtonPanel = *ObjectProperty->ContainerPtrToValuePtr<UHorizontalBox*>(this);
	//}
	//check(NavButtonPanel);

	// 开始加载游戏配置文件  获取要显示的导航按钮加载配置文件得到要加载的按钮名称
	/*通过按钮名称加载对应按钮的属性(加载Datatable)

		NavButtonAttr.ButtonType(FName)按钮的样式名称

		通过按钮的样式名称得到按钮的类型(AssetDataManager)

		通过按钮的类型实例化按钮再添加到NavButtonPanel并 且初始化该实例化的按钮(Init函数）*/
	TArray<FString> NavButtonNameArray;
	GConfig->GetArray(
		TEXT("NavButtonInit"),
		TEXT("NavButtonNames"),
		NavButtonNameArray,
		GGameIni
	);

	bool IsVertical;
	GConfig->GetBool(
		TEXT("NavButtonInit"),
		TEXT("ButtonPanel"),
		IsVertical,
		GGameIni
	);

	//开始生成按钮
	for (FString NavButtonNameStr : NavButtonNameArray)
	{
		FNavButtonAttr NavButtonAttr = FDataTableTool::GetNavButtonAttr(FName(*NavButtonNameStr));
		NavButtonAttrs.Add(NavButtonAttr);

		//NavButtonAttr.ButtonType
		UClass* NavButtonClass = ADataAssetMananger::RequestSyncLoadClass(this, NavButtonAttr.ButtonType);
		//只能创建UserWidget
		//UUserWidget* NavButton = CreateWidget<UUserWidget>(this->GetOwningPlayer(), NavButtonClass);

		//UWidget 包含UUserWidget UImage(不能单独存在，必须在一个WidgetTree下面),不能创建第一个
		UUserWidget* NavButton = this->WidgetTree->ConstructWidget<UUserWidget>(NavButtonClass);

		if (IsVertical)
		{
			UVerticalBoxSlot* VerticalBoxSlot = NavButtonVerticalPanel->AddChildToVerticalBox(NavButton);
			VerticalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			VerticalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		}
		else
		{
			UHorizontalBoxSlot* HorizontalBoxSlot = NavButtonHorizontalPanel->AddChildToHorizontalBox(NavButton);
			HorizontalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			HorizontalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		}

		UFunction* InitFun = NavButton->FindFunction(FName("Init"));
		if (InitFun)
		{
			//调用NavButton上的UFunction函数(Init)
			NavButton->ProcessEvent(InitFun, &NavButtonAttr);
		}

		//注册事件
		if (this->FindFunction(NavButtonAttr.ClickedEvent))
		{
			FEventManager::RegistEvent(NavButtonAttr.ClickedEvent, this, NavButtonAttr.ClickedEvent);
		}
	}

	//加载快捷栏
	FString ShortcutsWidgetType;//Type3
	GConfig->GetString(
		TEXT("ShortcutsInit"),
		TEXT("ShortcutsWidget"),
		ShortcutsWidgetType,
		GGameIni
	);

	FName CurrentShortcutsWidgetType = FName(*ShortcutsWidgetType);
	UClass* ShortcutsWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, CurrentShortcutsWidgetType);
	UUserWidget* ShortcutsWidget = this->WidgetTree->ConstructWidget<UUserWidget>(ShortcutsWidgetClass);

	UFunction* InitFun = ShortcutsWidget->FindFunction(FName("Init"));
	if (InitFun)
	{
		//调用ShortcutsWidget上的UFunction函数(Init)
		ShortcutsWidget->ProcessEvent(InitFun, &CurrentShortcutsWidgetType);
	}
	ShortcutsWidgetPanel->AddChild(ShortcutsWidget);
}

bool UHUDWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (InOperation->IsA<UBagGridDDO>())
	{
		int Index = Cast<UBagGridDDO>(InOperation)->FromGridIndex;
		FEventManager::TriggerEvent(FName("DropItemFromBagToGroundEvent"), &Index);

	}
	else if (InOperation->IsA<UShortcutsBarGridDDO>())
	{
		int Index = Cast<UShortcutsBarGridDDO>(InOperation)->FromGridIndex;
		FEventManager::TriggerEvent(FName("DropItemFromShortcutsToGroundEvent"), &Index);
	}
	else if (InOperation->IsA<UBagDDO>())
	{
		UBagDDO* BagDDO = Cast<UBagDDO>(InOperation);

		UUserWidget* DragBagWidget = Cast<UUserWidget>(BagDDO->DefaultDragVisual);
		DragBagWidget->AddToViewport();
		DragBagWidget->SetDesiredSizeInViewport(DragBagWidget->GetDesiredSize());
		InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
		//InGeometry.GetAbsoluteSize().X - DragBagWidget->GetDesiredSize().X;
		FVector2D NewLocation = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition()) - BagDDO->DragOffset;

		NewLocation.X = FMath::Min(NewLocation.X, InGeometry.GetAbsoluteSize().X - BagDDO->DragGeometry.GetAbsoluteSize().X);
		DragBagWidget->SetPositionInViewport(NewLocation, false);
	}

	return true;
}

void UHUDWidget::ToggleBagWidget(FName OpenWidgetType)
{
	if (IsValid(BagWidget))
	{
		CloseBag();
	}
	else
	{
		OpenBag(OpenWidgetType);
	}
}

void UHUDWidget::ToggleSettingWidget()
{
	UKismetSystemLibrary::PrintString(this, __FUNCTION__, true, false, FColor::Green, 100);

}

void UHUDWidget::ToggleEqumentWidget(FName OpenWidgetType)
{
	if (IsValid(EquipmentWidget))
	{
		CloseEquipment();
	}
	else
	{
		OpenEquipment(OpenWidgetType);
	}

}

void UHUDWidget::CloseBag()
{
	BagWidget->RemoveFromParent();

	BagWidget = nullptr;

	IsBagOpened = false;

	FEventManager::UnRegistEvent(FName("CloseBagEvent"), this);
	FEventManager::TriggerEvent(FName("BagOpened"), &IsBagOpened);
}

void UHUDWidget::OpenBag(FName OpenWidgetType)
{
	UClass* OpenWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, OpenWidgetType);

	BagWidget = WidgetTree->ConstructWidget<UUserWidget>(OpenWidgetClass);

	BagWidgetPanel->AddChild(BagWidget);
	UFunction* InitFun = BagWidget->FindFunction(FName("Init"));
	if (InitFun)
	{
		FString BagTypeStr;
		GConfig->GetString(
			TEXT("BagWidgetInit"),
			TEXT("BagType"),
			BagTypeStr,
			GGameIni
		);

		FName BagType = FName(*BagTypeStr);
		BagWidget->ProcessEvent(InitFun, &BagType);
	}

	IsBagOpened = true;

	FEventManager::RegistEvent(FName("CloseBagEvent"), this, FName("CloseBag"));
	FEventManager::TriggerEvent(FName("BagOpened"), &IsBagOpened);
}

void UHUDWidget::CloseEquipment()
{
	EquipmentWidget->RemoveFromParent();

	EquipmentWidget = nullptr;
}

void UHUDWidget::OpenEquipment(FName OpenWidgetType)
{
	UClass* OpenWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, OpenWidgetType);
	EquipmentWidget = WidgetTree->ConstructWidget<UUserWidget>(OpenWidgetClass);

	EquipmentWidgetPanel->AddChild(EquipmentWidget);
	UFunction* InitFun = EquipmentWidget->FindFunction(FName("Init"));
	if (InitFun)
	{
		FString EquipmentTypeStr;
		GConfig->GetString(
			TEXT("EquipmentInit"),
			TEXT("EquipmentWidget"),
			EquipmentTypeStr,
			GGameIni
		);

		FName Equipment = FName(*EquipmentTypeStr);
		EquipmentWidget->ProcessEvent(InitFun, &Equipment);
	}
}

