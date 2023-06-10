// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGM.h"
#include "Object/CommonTypes.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/EventManager/EventManager.h"



void AInventoryGM::BeginPlay()
{
	Super::BeginPlay();

	FEventManager::RegistEvent(FName("BagItemDropToGroundEvent"), this, FName("AddItem"));
	FEventManager::RegistEvent(FName("ShortcutsItemDropToGroundEvent"), this, FName("AddItem"));
	
	//生成地上掉落的物品
	//读取数据
	FGroundData GroundData = FFileTool::LoadGame().GroundData;
	//生成
	for (TPair<FName, FItemOnGroundData> KeyValuePair : GroundData.ItemOnGroundDataMap)
	{
		UpdateGroundItem(KeyValuePair.Value.ID, KeyValuePair.Value.Location, KeyValuePair.Key);

	}
}

void AInventoryGM::Destroyed()
{
	Super::Destroyed();
	FEventManager::UnRegistEvent(FName("BagItemDropToGroundEvent"), this);
	FEventManager::UnRegistEvent(FName("ShortcutsItemDropToGroundEvent"), this);
}

void AInventoryGM::AddItem(int ID, int Amount, FVector Location)
{
	//读取数据
	FGameSaveData GameSaveData = FFileTool::LoadGame();

	//确保重新生成的道具的Index不重名
	int IndexNum = 0;
	FName Index;
	//更改数据
	do
	{
		IndexNum++;
		Index = FName(*(FString("Item_") + FString::FromInt(IndexNum)));
	} while (GameSaveData.GroundData.ItemOnGroundDataMap.Contains(Index));

	GameSaveData.GroundData.ItemOnGroundDataMap.Add(Index, FItemOnGroundData(Index, ID, Amount, Location));
	FFileTool::SaveGame(GameSaveData);

	//触发一个事件  增加道具的事件   交互层监听该事件
	UpdateGroundItem(ID, Location, Index);
}

void AInventoryGM::UpdateGroundItem(int ID, FVector Location, FName Index)
{
	FName ItemOnGroundClassName = FDataTableTool::GetItemOnGroundAttr(IntToName(ID)).ItemClass;

	UClass* ItemOnGroundClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemOnGroundClassName);

	FTransform SpawnTF;
	SpawnTF.SetLocation(Location);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* ItemOnGround = GetWorld()->SpawnActor<AActor>(ItemOnGroundClass, SpawnTF, Params);
	UFunction* InitFun = ItemOnGround->FindFunction(FName("Init"));
	if (InitFun)
	{
		ItemOnGround->ProcessEvent(InitFun, &Index);
	}
}
