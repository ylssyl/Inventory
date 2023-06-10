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
	
	//���ɵ��ϵ������Ʒ
	//��ȡ����
	FGroundData GroundData = FFileTool::LoadGame().GroundData;
	//����
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
	//��ȡ����
	FGameSaveData GameSaveData = FFileTool::LoadGame();

	//ȷ���������ɵĵ��ߵ�Index������
	int IndexNum = 0;
	FName Index;
	//��������
	do
	{
		IndexNum++;
		Index = FName(*(FString("Item_") + FString::FromInt(IndexNum)));
	} while (GameSaveData.GroundData.ItemOnGroundDataMap.Contains(Index));

	GameSaveData.GroundData.ItemOnGroundDataMap.Add(Index, FItemOnGroundData(Index, ID, Amount, Location));
	FFileTool::SaveGame(GameSaveData);

	//����һ���¼�  ���ӵ��ߵ��¼�   ������������¼�
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
