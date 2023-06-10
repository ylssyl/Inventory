// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EquipmentActor.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Inventory/EventManager/EventManager.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEquipmentActor::AEquipmentActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEquipmentActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEquipmentActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AEquipmentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquipmentActor::Init(int InId)
{
}

void AEquipmentActor::UpdateEquipmentGrid(int InId)
{
}

void AEquipmentActor::EquipItemFromBagGrid(int InBagIndex, int InEquipId)
{
	FItemOnEquipmentGridAttr ItemOnEquipmentGridAttr = FDataTableTool::GetItemOnEquipmentGridAttr(IntToName(InEquipId));
	for (int i = 0; i < 6; i++)
	{
		if (FDataTableTool::GetEquipmentGridType1Attr(IntToName(i)).EquipmentGridPart == ItemOnEquipmentGridAttr.EquipmentPart)
		{
			struct
			{
				int BagIndex;
				int EquipIndex;
			}Params;
			Params.BagIndex = InBagIndex;
			Params.EquipIndex = i;
			FEventManager::TriggerEvent(FName("EquipItemEvent"), &Params);
		}
		
	}
	
}
