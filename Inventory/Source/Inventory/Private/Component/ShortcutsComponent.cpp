// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ShortcutsComponent.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"
#include "Inventory/EventManager/EventManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UShortcutsComponent::UShortcutsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShortcutsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FEventManager::RegistEvent(FName("ShortcutsGridDragToShortcutsGridEvent"), this, FName("ShortcutsGridDragToShortcutsGrid"));
	FEventManager::RegistEvent(FName("DropItemFromShortcutsToGroundEvent"), this, FName("ClearShortcutsGridByIndex"));
	FEventManager::RegistEvent(FName("ShortcutsOfShortcutsGridDragToBagGridEvent"), this, FName("ClearShortcutsGridByIndex"));
	FEventManager::RegistEvent(FName("ShortcutsOfBagGridDragToShortcutsGridEvent"), this, FName("BagGridDragToShortcutsGrid"));

}

void UShortcutsComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FEventManager::UnRegistEvent(FName("ShortcutsGridDragToShortcutsGridEvent"), this);
	FEventManager::UnRegistEvent(FName("DropItemFromShortcutsToGroundEvent"), this);
	FEventManager::UnRegistEvent(FName("ShortcutsOfShortcutsGridDragToBagGridEvent"), this);
	FEventManager::UnRegistEvent(FName("ShortcutsOfBagGridDragToShortcutsGridEvent"), this);
}


// Called every frame
void UShortcutsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShortcutsComponent::ShortcutsGridDragToShortcutsGrid(int FromIndex, int ToIndex)
{
	FShortcutsGridData FromGridData = FFileTool::GetShortcutsGridDataByIndex(FromIndex);
	FShortcutsGridData ToGridData = FFileTool::GetShortcutsGridDataByIndex(ToIndex);
	if (FromGridData.ShortcutsGridDataID != ToGridData.ShortcutsGridDataID)
	{
		//更改数据
		FFileTool::SetShortcutsGridDataByIndex(FromIndex, ToGridData);
		FFileTool::SetShortcutsGridDataByIndex(ToIndex, FromGridData);
	}
	else
	{
		if (FromIndex != ToIndex)
		{
			int Total = FromGridData.ShortcutsGridDataNum + ToGridData.ShortcutsGridDataNum;
			int MaxStackNum = FDataTableTool::GetItemOnShortcutsGridAttr(FName(*FString::FromInt(FromGridData.ShortcutsGridDataID))).MaxStackNum;
			ToGridData.ShortcutsGridDataNum = FMath::Min(Total, MaxStackNum);

			FromGridData.ShortcutsGridDataNum = Total - ToGridData.ShortcutsGridDataNum;
			FromGridData.ShortcutsGridDataID = FromGridData.ShortcutsGridDataNum == 0 ? 0 : FromGridData.ShortcutsGridDataID;

			//更改数据
			FFileTool::SetShortcutsGridDataByIndex(FromIndex, FromGridData);
			FFileTool::SetShortcutsGridDataByIndex(ToIndex, ToGridData);
		}

	}

	//触发事件
	FEventManager::TriggerEvent(FName("ShortcutsGridDragToShortcutsGridFinishedEvent"), &FromIndex);
	FEventManager::TriggerEvent(FName("ShortcutsGridDragToShortcutsGridFinishedEvent"), &ToIndex);
}

void UShortcutsComponent::BagGridDragToShortcutsGrid(int FromIndex, int ToIndex)
{
	//FItemOnEquipmentGridAttr ItemOnEquipmentGridAttr = FDataTableTool::GetItemOnEquipmentGridAttr(IntToName(FromIndex));
	//if (ItemOnEquipmentGridAttr.Size.X) return;

	FBagGridData FromGridData = FFileTool::GetPlayerBagGridDataByIndex(FromIndex);
	FShortcutsGridData ToGridData = FFileTool::GetShortcutsGridDataByIndex(ToIndex);
	if (FromGridData.ID != ToGridData.ShortcutsGridDataID)
	{
		//更改数据
		FFileTool::SwapShortoutsGridDataAndBagGridData(FromGridData, ToGridData);
		FFileTool::SetPlayerBagGridDataByIndex(FromIndex, FromGridData);
		FFileTool::SetShortcutsGridDataByIndex(ToIndex, ToGridData);
	}
	else
	{
		if (FromIndex != ToIndex)
		{
			int Total = FromGridData.Num + ToGridData.ShortcutsGridDataNum;
			int MaxStackNum = FDataTableTool::GetItemOnShortcutsGridAttr(FName(*FString::FromInt(FromGridData.ID))).MaxStackNum;
			ToGridData.ShortcutsGridDataNum = FMath::Min(Total, MaxStackNum);

			FromGridData.Num = Total - ToGridData.ShortcutsGridDataNum;
			FromGridData.ID = FromGridData.Num == 0 ? 0 : FromGridData.ID;

			//更改数据
			FFileTool::SetPlayerBagGridDataByIndex(FromIndex, FromGridData);
			FFileTool::SetShortcutsGridDataByIndex(ToIndex, ToGridData);
		}

	}

	//触发事件
	FEventManager::TriggerEvent(FName("BagOfBagGridDragToShortoutsGridFinishedEvent"), &FromIndex);
	FEventManager::TriggerEvent(FName("ShortoutsOfBagGridDragToShortoutsGridFinishedEvent"), &ToIndex);
}

void UShortcutsComponent::ClearShortcutsGridByIndex(int InShortcutsIndex)
{
	FShortcutsGridData ShortcutsGridData = FFileTool::GetShortcutsGridDataByIndex(InShortcutsIndex);

	FFileTool::ClearPlayerShortoutsGridByIndex(InShortcutsIndex);
	FEventManager::TriggerEvent(FName("ClearShortcutsGridByIndexFinishedEvent"), &InShortcutsIndex);


	struct
	{
		int ID;
		int Amount;
		FVector Location;
	}Params;
	Params.ID = ShortcutsGridData.ShortcutsGridDataID;
	Params.Amount = ShortcutsGridData.ShortcutsGridDataNum;
	Params.Location = GetOwner()->GetActorLocation();
	FEventManager::TriggerEvent(FName("ShortcutsItemDropToGroundEvent"), &Params);//update
}

