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

	
	Super::NativeConstruct();//��������
}

void UHUDWidget::NativeDestruct()
{
	//ע���¼�
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
	//{//�÷�������ȡ��ͼ��widget���(meta = (BindWidget)��ԭ��)
	//	UObjectProperty* ObjectProperty = Cast<UObjectProperty>(Property);
	//	NavButtonPanel = *ObjectProperty->ContainerPtrToValuePtr<UHorizontalBox*>(this);
	//}
	//check(NavButtonPanel);

	// ��ʼ������Ϸ�����ļ�  ��ȡҪ��ʾ�ĵ�����ť���������ļ��õ�Ҫ���صİ�ť����
	/*ͨ����ť���Ƽ��ض�Ӧ��ť������(����Datatable)

		NavButtonAttr.ButtonType(FName)��ť����ʽ����

		ͨ����ť����ʽ���Ƶõ���ť������(AssetDataManager)

		ͨ����ť������ʵ������ť����ӵ�NavButtonPanel�� �ҳ�ʼ����ʵ�����İ�ť(Init������*/
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

	//��ʼ���ɰ�ť
	for (FString NavButtonNameStr : NavButtonNameArray)
	{
		FNavButtonAttr NavButtonAttr = FDataTableTool::GetNavButtonAttr(FName(*NavButtonNameStr));
		NavButtonAttrs.Add(NavButtonAttr);

		//NavButtonAttr.ButtonType
		UClass* NavButtonClass = ADataAssetMananger::RequestSyncLoadClass(this, NavButtonAttr.ButtonType);
		//ֻ�ܴ���UserWidget
		//UUserWidget* NavButton = CreateWidget<UUserWidget>(this->GetOwningPlayer(), NavButtonClass);

		//UWidget ����UUserWidget UImage(���ܵ������ڣ�������һ��WidgetTree����),���ܴ�����һ��
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
			//����NavButton�ϵ�UFunction����(Init)
			NavButton->ProcessEvent(InitFun, &NavButtonAttr);
		}

		//ע���¼�
		if (this->FindFunction(NavButtonAttr.ClickedEvent))
		{
			FEventManager::RegistEvent(NavButtonAttr.ClickedEvent, this, NavButtonAttr.ClickedEvent);
		}
	}

	//���ؿ����
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
		//����ShortcutsWidget�ϵ�UFunction����(Init)
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

