// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BagComponent.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"
#include "Inventory/EventManager/EventManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBagComponent::UBagComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBagComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	FEventManager::RegistEvent(FName("StartCheckBagGrid"), this, FName("StartCheckBagItem"));
	FEventManager::RegistEvent(FName("EndCheckBagGrid"), this, FName("EndCheckBagItem"));
	FEventManager::RegistEvent(FName("BagSortItemEvent"), this, FName("SortItem"));
	FEventManager::RegistEvent(FName("ItemBePickUpEvent"), this, FName("OnPickItem"));
	FEventManager::RegistEvent(FName("BagGridDragToBagGridEvent"), this, FName("BagGridDragToBagGrid"));
	FEventManager::RegistEvent(FName("DropItemFromBagToGroundEvent"), this, FName("ClearBagGridByIndex"));
	FEventManager::RegistEvent(FName("BagOfShortcutsGridDragToBagGridEvent"), this, FName("ShortcutsGridDragToBagGrid"));
	FEventManager::RegistEvent(FName("aaa"), this, FName("AddItem"));
	FEventManager::RegistEvent(FName("EquipItemEvent"), this, FName("BagGridDragToEquipmentGrid"));
}

void UBagComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (EEndPlayReason::Destroyed)
	{
		FEventManager::UnRegistEvent(FName("StartCheckBagGrid"), this);
		FEventManager::UnRegistEvent(FName("EndCheckBagGrid"), this);
		FEventManager::UnRegistEvent(FName("BagSortItemEvent"), this);
		FEventManager::UnRegistEvent(FName("ItemBePickUpEvent"), this);
		FEventManager::UnRegistEvent(FName("BagGridDragToBagGridEvent"), this);
		FEventManager::UnRegistEvent(FName("DropItemFromBagToGroundEvent"), this);
		FEventManager::UnRegistEvent(FName("BagOfShortcutsGridDragToBagGridEvent"), this);
		FEventManager::UnRegistEvent(FName("aaa"), this);
		FEventManager::UnRegistEvent(FName("EquipItemEvent"), this);
	}
}


// Called every frame
void UBagComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBagComponent::SortItem()
{
	//��ȡ����
	TArray<FBagGridData> BagGridDatas = FFileTool::GetPlayerBagGridDatas();
	//��ӵ�еĵ��ߵ�����
	//Key ID  Value Total
	TMap<int, int> TotalNums; //10011  6    10011   7
	for (FBagGridData BagGridData : BagGridDatas)
	{
		if (BagGridData.ID == 0) continue;

		if (!TotalNums.Contains(BagGridData.ID))
		{
			TotalNums.Add(BagGridData.ID, BagGridData.Num);
		}
		else
		{
			(*TotalNums.Find(BagGridData.ID)) += BagGridData.Num;
		}
	}

	TotalNums.KeySort([](int Left, int Right) {return Left < Right; });

	TArray<FBagGridData> NewBagGridDatas;
	for (TPair<int, int> KeyValuePair : TotalNums)
	{
		int MaxStackNum = FDataTableTool::GetItemOnBagGridAttr(IntToName(KeyValuePair.Key)).MaxStackNum;
		int FullGridNum = KeyValuePair.Value / MaxStackNum;
		int FreeGridItemNum = KeyValuePair.Value % MaxStackNum;
		for (int i = 0; i < FullGridNum; i++)
		{
			NewBagGridDatas.Add(FBagGridData(KeyValuePair.Key, MaxStackNum));
		}
		if (FreeGridItemNum > 0)
		{
			NewBagGridDatas.Add(FBagGridData(KeyValuePair.Key, FreeGridItemNum));
		}
	}
	int EmptyGridNum = BagGridDatas.Num() - NewBagGridDatas.Num();
	for (int i = 0; i < EmptyGridNum; i++)
	{
		NewBagGridDatas.Add(FBagGridData());
	}//�����ĸ����ÿ�
	//��������
	FFileTool::SetPlayerBagGridDatas(NewBagGridDatas);

	FEventManager::TriggerEvent(FName("BagItemSortFinishedEvent"), NULL);
}

int UBagComponent::AddItem(int ID, int Amount)
{
	//��ȡ����
	TArray<FBagGridData> BagGridDatas = FFileTool::GetPlayerBagGridDatas();
	int MaxStackNum = FDataTableTool::GetItemOnBagGridAttr(FName(*FString::FromInt(ID))).MaxStackNum;
	int FoundIndex = FindFreeGridIndex(ID);
	if (FoundIndex != -1)//�ҵ��������׶����ĸ���
	{
		int Total = BagGridDatas[FoundIndex].Num + Amount;  //7+4
		BagGridDatas[FoundIndex].Num = FMath::Min(Total, MaxStackNum);
		//ʣ������
		Amount = Total - BagGridDatas[FoundIndex].Num;
		FBagGridData NewBagGridData = BagGridDatas[FoundIndex];
		//��������
		FFileTool::SetPlayerBagGridDataByIndex(FoundIndex, NewBagGridData);
		//����һ���¼� BagAddItemEvent
		FEventManager::TriggerEvent(FName("BagAddItemEvent"), &FoundIndex);

		if (Amount == 0)
		{
			return 0;//���е�ͬ���͵ĸ��ӿ����������Ҫʰȡ������
		}
		else
		{
			return AddItem(ID, Amount);
		}

	}
	else//û���ҵ������׶����ĸ���
	{
		//��ʼ�ҿո���
		FoundIndex = FindEmptyGridIndex();
		if (FoundIndex != -1)//�ҵ��˿ո���
		{
			FBagGridData NewBagGridData;
			NewBagGridData.ID = ID;
			NewBagGridData.Num = FMath::Min(Amount, MaxStackNum);
			Amount -= NewBagGridData.Num;

			//��������
			FFileTool::SetPlayerBagGridDataByIndex(FoundIndex, NewBagGridData);
			//����һ���¼� BagAddItemEvent
			FEventManager::TriggerEvent(FName("BagAddItemEvent"), &FoundIndex);

			
			if (Amount == 0)
			{
				return 0;//ͨ���ո���������Ҫʰȡ��ʣ�������
			}
			else
			{
				return AddItem(ID, Amount);
			}
		}
		else//û���ҵ������׶����ĸ��� �� û���ҵ��ո���
		{
			return Amount;
		}
	}

	check(false);
	return -9999;
}

void UBagComponent::OnPickItem(FName ItemIndex)
{
	//��ȡ����
	FItemOnGroundData* ItemOnGroundData = FFileTool::LoadGame().GroundData.ItemOnGroundDataMap.Find(ItemIndex);
	if (ItemOnGroundData)
	{
		int LeftNum = this->AddItem(ItemOnGroundData->ID, ItemOnGroundData->Num);
		//��������
		FFileTool::SetItemOnGroundDataNumByIndex(ItemIndex, LeftNum);

		FEventManager::TriggerEvent(FName("ItemBePickUpFinishedEvent"), &ItemIndex);
	}
}

void UBagComponent::BagGridDragToBagGrid(int FromIndex, int ToIndex)
{
	FBagGridData FromGridData = FFileTool::GetPlayerBagGridDataByIndex(FromIndex);
	FBagGridData ToGridData = FFileTool::GetPlayerBagGridDataByIndex(ToIndex);
	if (FromGridData.ID != ToGridData.ID)
	{
		//��������
		FFileTool::SetPlayerBagGridDataByIndex(FromIndex, ToGridData);
		FFileTool::SetPlayerBagGridDataByIndex(ToIndex, FromGridData);


	}
	else
	{
		if (FromIndex != ToIndex)
		{
			int Total = FromGridData.Num + ToGridData.Num;
			int MaxStackNum = FDataTableTool::GetItemOnBagGridAttr(FName(*FString::FromInt(FromGridData.ID))).MaxStackNum;
			ToGridData.Num = FMath::Min(Total, MaxStackNum);

			FromGridData.Num = Total - ToGridData.Num;
			FromGridData.ID = FromGridData.Num == 0 ? 0 : FromGridData.ID;

			//��������
			FFileTool::SetPlayerBagGridDataByIndex(FromIndex, FromGridData);
			FFileTool::SetPlayerBagGridDataByIndex(ToIndex, ToGridData);
		}

	}

	//�����¼�
	FEventManager::TriggerEvent(FName("BagGridDragToBagGridFinishedEvent"), &FromIndex);
	FEventManager::TriggerEvent(FName("BagGridDragToBagGridFinishedEvent"), &ToIndex);
}

void UBagComponent::ClearBagGridByIndex(int Index)
{
	FBagGridData BagGridData = FFileTool::GetPlayerBagGridDataByIndex(Index);

	FFileTool::ClearPlayerBagGridByIndex(Index);
	FEventManager::TriggerEvent(FName("ClearBagGridByIndexFinishedEvent"), &Index);

	//AddItem(int ID, int Amount, FVector Location)

	struct
	{
		int ID;
		int Amount;
		FVector Location;
	}Params;
	Params.ID = BagGridData.ID;
	Params.Amount = BagGridData.Num;
	Params.Location = GetOwner()->GetActorLocation();
	FEventManager::TriggerEvent(FName("BagItemDropToGroundEvent"), &Params);
}

void UBagComponent::ShortcutsGridDragToBagGrid(int FromIndex, int ToIndex)
{
	FShortcutsGridData FromGridData = FFileTool::GetShortcutsGridDataByIndex(FromIndex);
	FBagGridData ToGridData = FFileTool::GetPlayerBagGridDataByIndex(ToIndex);
	if (FromGridData.ShortcutsGridDataID != ToGridData.ID)
	{
		//��������
		FFileTool::SwapShortoutsGridDataAndBagGridData(ToGridData, FromGridData);
		FFileTool::SetShortcutsGridDataByIndex(FromIndex, FromGridData);
		FFileTool::SetPlayerBagGridDataByIndex(ToIndex, ToGridData);
	}
	else
	{
		if (FromIndex != ToIndex)
		{
			int Total = FromGridData.ShortcutsGridDataNum + ToGridData.Num;
			int MaxStackNum = FDataTableTool::GetItemOnShortcutsGridAttr(FName(*FString::FromInt(FromGridData.ShortcutsGridDataID))).MaxStackNum;
			ToGridData.Num = FMath::Min(Total, MaxStackNum);

			FromGridData.ShortcutsGridDataID = Total - ToGridData.Num;
			FromGridData.ShortcutsGridDataID = FromGridData.ShortcutsGridDataNum == 0 ? 0 : FromGridData.ShortcutsGridDataID;

			//��������
			FFileTool::SetShortcutsGridDataByIndex(FromIndex, FromGridData);
			FFileTool::SetPlayerBagGridDataByIndex(ToIndex, ToGridData);
		}

	}

	//�����¼�
	FEventManager::TriggerEvent(FName("ShortcutsOfShortcutsGridDragToBagGridFinishedEvent"), &FromIndex);
	FEventManager::TriggerEvent(FName("BagOfShortcutsGridDragToBagGridFinishedEvent"), &ToIndex);
}

void UBagComponent::BagGridDragToEquipmentGrid(int FromIndex, int ToIndex)
{
	FBagGridData FromGridData = FFileTool::GetPlayerBagGridDataByIndex(FromIndex);
	FEquipmentGridData ToGridData = FFileTool::GetEquipmentGridDataByIndex(ToIndex);
	FEquipmentGridType1Attr EquipmentGridType1Attr = FDataTableTool::GetEquipmentGridType1Attr(IntToName(ToIndex));
	FItemCommonAttr FromGridDataCommonAttr = FDataTableTool::GetItemCommonAttr(IntToName(FromGridData.ID));
	if (FromGridDataCommonAttr.UsedItem == FName("EquipmentActor") && FromGridData.ID != ToGridData.EquipmentGridDataID)
	{
		FItemOnEquipmentGridAttr ItemOnEquipmentGridAttr = FDataTableTool::GetItemOnEquipmentGridAttr(IntToName(FromGridData.ID));
		if (ItemOnEquipmentGridAttr.EquipmentPart == EquipmentGridType1Attr.EquipmentGridPart)
		{
			FFileTool::SwapEquipmentGridDataAndBagGridData(FromGridData, ToGridData);
			FFileTool::SetEquipmentGridDataByIndex(ToIndex, ToGridData);
			FFileTool::SetPlayerBagGridDataByIndex(FromIndex, FromGridData);
		}
	}
	FEventManager::TriggerEvent(FName("BagOfShortcutsGridDragToBagGridFinishedEvent"), &FromIndex);
	FEventManager::TriggerEvent(FName("EquipItemFinishedEvent"), &ToIndex);
}

//��ӽ����������Ҳ��������������ĸ��ӵ�����
int UBagComponent::FindFreeGridIndex(int ID)
{
	//��ȡ����
	TArray<FBagGridData> BagGridDatas = FFileTool::GetPlayerBagGridDatas();
	int MaxStackNum = FDataTableTool::GetItemOnBagGridAttr(FName(*FString::FromInt(ID))).MaxStackNum;

	int MaxNum = 0;
	int FoundIndex = -1;

	for (int Index = 0; Index < BagGridDatas.Num(); Index++)
	{
		if (BagGridDatas[Index].ID == ID && BagGridDatas[Index].Num != MaxStackNum)
		{
			if (MaxNum < BagGridDatas[Index].Num)
			{
				MaxNum = BagGridDatas[Index].Num;
				FoundIndex = Index;
			}
		}
	}
	return FoundIndex;
}

int UBagComponent::FindEmptyGridIndex()
{
	//��ȡ����
	TArray<FBagGridData> BagGridDatas = FFileTool::GetPlayerBagGridDatas();
	for (int Index = 0; Index < BagGridDatas.Num(); Index++)
	{
		if (BagGridDatas[Index].ID == 0) {
			return Index;
		}
	}

	return -1;
}

void UBagComponent::StartCheckBagItem(int InGridIndex)
{
	FBagGridData BagGridData = FFileTool::GetPlayerBagGridDataByIndex(InGridIndex);
	if (!FFileTool::IsHadItem(BagGridData.ID))
	{
		//���ID��HadItem
		FFileTool::AddHadItem(BagGridData.ID);
		//�����¼����ü��������Լ�����
		FEventManager::TriggerEvent(FName("HadCheckBagItemEvent"), &InGridIndex);
	}
}

void UBagComponent::EndCheckBagItem(int InGridIndex)
{
}

