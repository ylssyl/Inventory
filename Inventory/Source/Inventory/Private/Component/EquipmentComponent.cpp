// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EquipmentComponent.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"
#include "Inventory/EventManager/EventManager.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	//EquipInit();
	// ...
	FEventManager::RegistEvent(FName("EquipItemLoadEvent"), this, FName("EquipUpdate"));
	FEventManager::RegistEvent(FName("EquipItemLoad1Event"), this, FName("EquipUpdate1"));

}

void UEquipmentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FEventManager::UnRegistEvent(FName("EquipItemLoadEvent"), this);
	FEventManager::UnRegistEvent(FName("EquipItemLoad1Event"), this);
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


//UClass* EquipmentActorClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemCommonAttr.UsedItem);
//EquipmentActorClass->GetDefaultObject();//反射
int UEquipmentComponent::Query(int InEquipID)
{
	FItemOnEquipmentGridAttr ItemOnEquipmentGridAttr = FDataTableTool::GetItemOnEquipmentGridAttr(IntToName(InEquipID));
	for (int i = 0; i < 6; i++)
	{
		FDataTableTool::GetEquipmentGridType1Attr(IntToName(i)).EquipmentGridPart = ItemOnEquipmentGridAttr.EquipmentPart;
		return i;
	}
	return -1;
}

bool UEquipmentComponent::AddEquip(int ID, int Amount)
{
	int Index = Query(ID);
	FEquipmentGridData NewEquipmentGridData = FEquipmentGridData(ID, Amount);
	FFileTool::SetEquipmentGridDataByIndex(Index, NewEquipmentGridData);
	return true;
}

void UEquipmentComponent::EquipUpdate(int InId)
{
	FEventManager::TriggerEvent(FName("LoadEquipmentEvent"), &InId);//给character自己加载
}

void UEquipmentComponent::EquipUpdate1(int Id, int InIndex)
{
	if (Id != 0)
		EquipUpdate(Id);
	else if(Id == 0)
	{
		FName InEquipmentPart = GetEquipmentPart(InIndex);
		FEventManager::TriggerEvent(FName("ClearEquipmentSkeletalEvent"), &InEquipmentPart);//清空装备骨骼
	}
}

void UEquipmentComponent::EquipInit()
{
	for (int i = 0; i < 6; i++)
	{
		FEquipmentGridData EquipmentGridData = FFileTool::GetEquipmentGridDataByIndex(i);

		if (EquipmentGridData.EquipmentGridDataID != 0)
		{
			EquipUpdate(EquipmentGridData.EquipmentGridDataID);
		}

	}
}

FName UEquipmentComponent::GetEquipmentPart(int InIndex)
{
	FEquipmentGridType1Attr EquipmentGridType1Attr = FDataTableTool::GetEquipmentGridType1Attr(IntToName(InIndex));
	return EquipmentGridType1Attr.EquipmentGridPart;
}

