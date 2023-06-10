// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/BagWidgetType1.h"

#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Inventory/EventManager/EventManager.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Object/BagDDO.h"

void UBagWidgetType1::Init(FName BagType)
{
	FBagWidgetType1Attr BagWidgetType1Attr = FDataTableTool::GetBagWidgetType1Attr(BagType);
	
	this->SetVisibility(ESlateVisibility::Collapsed);

	BagSizeBox->SetWidthOverride(BagWidgetType1Attr.BagSize.X);
	BagSizeBox->SetHeightOverride(BagWidgetType1Attr.BagSize.Y);

	ADataAssetMananger::RequestAsyncLoadObjects
	(
		this, 
		{
			BagWidgetType1Attr.CloseButtonNormalIcon,
			BagWidgetType1Attr.CloseButtonHoverIcon,
			BagWidgetType1Attr.CloseButtonPressedIcon
		},
		[this](TArray<UObject*> Assets) {

			FButtonStyle* ButtonStyle = &(CloseButton->WidgetStyle);
			ButtonStyle->Normal.SetResourceObject(Assets[0]);
			ButtonStyle->Normal.DrawAs = ESlateBrushDrawType::Image;

			ButtonStyle->Hovered.SetResourceObject(Assets[1]);
			ButtonStyle->Hovered.DrawAs = ESlateBrushDrawType::Image;

			ButtonStyle->Pressed.SetResourceObject(Assets[2]);
			ButtonStyle->Pressed.DrawAs = ESlateBrushDrawType::Image;

			this->SetVisibility(ESlateVisibility::Visible);
		}
	); 
	
	ADataAssetMananger::RequestAsyncLoadObject(this, FName("SortItemButtonTex"),
		[this](UObject* Assets)
		{
			FButtonStyle* ButtonStyle = &(SortItemButton->WidgetStyle);
			ButtonStyle->Normal.SetResourceObject(Assets);
			ButtonStyle->Normal.DrawAs = ESlateBrushDrawType::Image;
		}
		);
	ADataAssetMananger::RequestAsyncLoadObject(this, FName("SortItemButtonTex"),
		[this](UObject* Assets)
		{
			FButtonStyle* ButtonStyle = &(SortItemButton->WidgetStyle);
			ButtonStyle->Hovered.SetResourceObject(Assets);
			ButtonStyle->Hovered.DrawAs = ESlateBrushDrawType::Image;
		}
	);
	ADataAssetMananger::RequestAsyncLoadObject(this, FName("SortItemButtonTex"),
		[this](UObject* Assets)
		{
			FButtonStyle* ButtonStyle = &(SortItemButton->WidgetStyle);
			ButtonStyle->Pressed.SetResourceObject(Assets);
			ButtonStyle->Pressed.DrawAs = ESlateBrushDrawType::Image;
		}
	);

	//创建子对象(格子)
	int GridNum;
	/*GConfig->GetInt(
		TEXT("BagWidgetInit"),
		TEXT("GridNum"),
		GridNum,
		GGameIni
	);*/

	FGameSaveData GameSaveData = FFileTool::LoadGame();
	GridNum = GameSaveData.PlayerData.BagData.GridNums;

	check(GridNum);
	UClass* GridWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, BagWidgetType1Attr.GridType);
	for (int GridIndex = 0; GridIndex < GridNum; GridIndex++)
	{
		UUserWidget* GridWidget = WidgetTree->ConstructWidget<UUserWidget>(GridWidgetClass);
		check(GridWidget);
		GridsPanel->AddChildToWrapBox(GridWidget);

		UFunction* InitFun = GridWidget->FindFunction(FName("Init"));
		if (InitFun)
		{
			GridWidget->ProcessEvent(InitFun, &GridIndex);
		}
	}

	ParentPanelWidget = this->GetParent();
}

void UBagWidgetType1::OnCloseBagWidget()
{
	FEventManager::TriggerEvent(FName("CloseBagEvent"), NULL);
}

void UBagWidgetType1::OnSortItem()
{
	FEventManager::TriggerEvent(FName("BagSortItemEvent"), NULL);
}

void UBagWidgetType1::NativeConstruct()
{
	CloseButton->OnClicked.AddDynamic(this, &UBagWidgetType1::OnCloseBagWidget);
	SortItemButton->OnClicked.AddDynamic(this, &UBagWidgetType1::OnSortItem);

	Super::NativeConstruct();
}

void UBagWidgetType1::NativeDestruct()
{
	CloseButton->OnClicked.RemoveAll(this);
	SortItemButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

FReply UBagWidgetType1::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//标记为已经处理,UI处理之后，游戏就不会处理了
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton);

		return EventReply.NativeReply;
	}

	return FReply::Handled();
}

void UBagWidgetType1::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UBagDDO* BagDDO = Cast<UBagDDO>(UWidgetBlueprintLibrary::CreateDragDropOperation(UBagDDO::StaticClass()));

	BagDDO->Pivot = EDragPivot::MouseDown;
	BagDDO->DefaultDragVisual = this;
	OutOperation = BagDDO;
	//InMouseEvent.GetScreenSpacePosition();获取到鼠标相对于屏幕的坐标
	BagDDO->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	BagDDO->DragGeometry = InGeometry;
	

	this->RemoveFromParent();
	BagDDO->OnDragCancelled.AddDynamic(this, &UBagWidgetType1::OnBagWidgetDragCancelled);
}

bool UBagWidgetType1::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return true;
}

void UBagWidgetType1::OnBagWidgetDragCancelled(UDragDropOperation* Operation)
{
	ParentPanelWidget->AddChild(this);
}
