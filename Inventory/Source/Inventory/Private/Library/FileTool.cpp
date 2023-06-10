// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/FileTool.h"
#include "Misc/Paths.h"  
#include "Misc/FileHelper.h"  
#include "Library/JsonTool.h"
#include "Misc/ConfigCacheIni.h"
#include "Library/DataTableTool.h"
#include "Kismet/GameplayStatics.h"

void FFileTool::ClearPlayerBagGridByIndex(int Index)
{
	FGameSaveData GameSaveData = LoadGame();
	GameSaveData.PlayerData.BagData.GridDatas[Index] = FBagGridData(0, 0);
	SaveGame(GameSaveData);
}

void FFileTool::ClearPlayerShortoutsGridByIndex(int Index)
{
	FGameSaveData GameSaveData = LoadGame();
	GameSaveData.ShortcutsData.ShortcutsGridDatas[Index] = FShortcutsGridData(0, 0);
	SaveGame(GameSaveData);
}

void FFileTool::ClearPlayerEquipmentGridByIndex(int Index)
{
	FGameSaveData GameSaveData = LoadGame();
	FEquipmentGridType1Attr EquipmentGridType1Attr = FDataTableTool::GetEquipmentGridType1Attr(IntToName(Index));
	GameSaveData.EquipmentData.EquipmentDataMap[EquipmentGridType1Attr.EquipmentGridPart] = FEquipmentGridData();
	SaveGame(GameSaveData);
}

void FFileTool::SwapShortoutsGridDataAndBagGridData(FBagGridData& InBagGridData, FShortcutsGridData& InShortcutsGridData)
{
	int Id = 0;
	int Number = 0;
	Id = InBagGridData.ID;
	Number = InBagGridData.Num;
	InBagGridData.ID = InShortcutsGridData.ShortcutsGridDataID;
	InBagGridData.Num = InShortcutsGridData.ShortcutsGridDataNum;
	InShortcutsGridData.ShortcutsGridDataID = Id;
	InShortcutsGridData.ShortcutsGridDataNum = Number;

}

void FFileTool::SwapEquipmentGridDataAndBagGridData(FBagGridData& InBagGridData, FEquipmentGridData& InEquipmentGridData)
{
	int Id = 0;
	int Number = 0;
	Id = InBagGridData.ID;
	Number = InBagGridData.Num;
	InBagGridData.ID = InEquipmentGridData.EquipmentGridDataID;
	InBagGridData.Num = InEquipmentGridData.EquipmentGridDataNum;
	InEquipmentGridData.EquipmentGridDataID = Id;
	InEquipmentGridData.EquipmentGridDataNum = Number;
}

bool FFileTool::SetItemOnGroundDataNumByIndex(FName InIndex, int NewNum)
{
	FGameSaveData GameSaveData = LoadGame();
	FItemOnGroundData* ItemOnGroundData = GameSaveData.GroundData.ItemOnGroundDataMap.Find(InIndex);
	if (ItemOnGroundData)
	{
		if (NewNum == 0)
		{
			GameSaveData.GroundData.ItemOnGroundDataMap.Remove(InIndex);
		}
		else
		{
			ItemOnGroundData->Num = NewNum;
		}
		SaveGame(GameSaveData);
		return true;
	}
	return false;
}

TArray<FBagGridData> FFileTool::GetPlayerBagGridDatas()
{
	return LoadGame().PlayerData.BagData.GridDatas;
}

void FFileTool::SetPlayerBagGridDatas(TArray<FBagGridData> NewBagGridDatas)
{
	FGameSaveData GameSaveDate = LoadGame();
	GameSaveDate.PlayerData.BagData.GridDatas = NewBagGridDatas;
	SaveGame(GameSaveDate);
}

bool FFileTool::IsHadItem(int ID)
{
	FGameSaveData GameSaveDate = LoadGame();

	return GameSaveDate.PlayerData.BagData.HadItems.Contains(ID);
}

void FFileTool::AddHadItem(int ID)
{
	FGameSaveData GameSaveDate = LoadGame();

	GameSaveDate.PlayerData.BagData.HadItems.AddUnique(ID);

	SaveGame(GameSaveDate);
}

FBagGridData FFileTool::GetPlayerBagGridDataByIndex(int Index)
{
	FBagData BagData = LoadGame().PlayerData.BagData;
	checkf((Index >= 0) & (Index < BagData.GridDatas.Num()), TEXT("Array index out of bounds: %i from an array of size %i"), Index, BagData.GridDatas.Num());

	return BagData.GridDatas[Index];
}

void FFileTool::SetPlayerBagGridDataByIndex(int FoundIndex, FBagGridData NewBagGridData)
{
	FGameSaveData GameSaveData = LoadGame();
	FBagData BagData = GameSaveData.PlayerData.BagData;
	checkf((FoundIndex >= 0) & (FoundIndex < BagData.GridDatas.Num()), TEXT("Array index out of bounds: %i from an array of size %i"), FoundIndex, BagData.GridDatas.Num());
	GameSaveData.PlayerData.BagData.GridDatas[FoundIndex] = NewBagGridData;
	SaveGame(GameSaveData);
}

FShortcutsGridData FFileTool::GetShortcutsGridDataByIndex(int Index)
{
	FShortcutsData ShortcutsData = LoadGame().ShortcutsData;

	checkf((Index >= 0) & (Index < ShortcutsData.ShortcutsGridDatas.Num()), TEXT("Array index out of bounds: %i from an array of size %i"), Index, ShortcutsData.ShortcutsGridDatas.Num());

	return ShortcutsData.ShortcutsGridDatas[Index];
}

void FFileTool::SetShortcutsGridDataByIndex(int FoundIndex, FShortcutsGridData NewShortcutsGridData)
{
	FGameSaveData GameSaveData = LoadGame();
	FShortcutsData ShortcutsData = GameSaveData.ShortcutsData;
	checkf((FoundIndex >= 0) & (FoundIndex < ShortcutsData.ShortcutsGridDatas.Num()), TEXT("Array index out of bounds: %i from an array of size %i"), FoundIndex, ShortcutsData.ShortcutsGridDatas.Num());
	GameSaveData.ShortcutsData.ShortcutsGridDatas[FoundIndex] = NewShortcutsGridData;
	SaveGame(GameSaveData);
}

FEquipmentGridData FFileTool::GetEquipmentGridDataByIndex(int Index)
{
	FEquipmentData EquipmentData = LoadGame().EquipmentData;
	FEquipmentGridType1Attr EquipmentGridType1Attr = FDataTableTool::GetEquipmentGridType1Attr(IntToName(Index));
	checkf((Index >= 0) & (Index < EquipmentData.EquipmentDataMap.Num()), TEXT("Array index out of bounds: %i from an array of size %i"), Index, EquipmentData.EquipmentDataMap.Num());
	return EquipmentData.EquipmentDataMap[EquipmentGridType1Attr.EquipmentGridPart];
}

void FFileTool::SetEquipmentGridDataByIndex(int FoundIndex, FEquipmentGridData NewEquipmentGridData)
{
	FGameSaveData GameSaveData = LoadGame();
	FEquipmentData EquipmentData = LoadGame().EquipmentData;
	FEquipmentGridType1Attr EquipmentGridType1Attr = FDataTableTool::GetEquipmentGridType1Attr(IntToName(FoundIndex));
	checkf((FoundIndex >= 0) & (FoundIndex < EquipmentData.EquipmentDataMap.Num()), TEXT("Array index out of bounds: %i from an array of size %i"), FoundIndex, EquipmentData.EquipmentDataMap.Num());
	GameSaveData.EquipmentData.EquipmentDataMap[EquipmentGridType1Attr.EquipmentGridPart] = NewEquipmentGridData;
	SaveGame(GameSaveData);
}

FGameSaveData FFileTool::LoadGame(FString RealtivePath, FString FileName)
{
	//Json 1kb Xml  protobuf 0.1kb    

	FString JsonStr;
	LoadFileToStr(JsonStr, RealtivePath, FileName);
	//UKismetSystemLibrary::PrintString(0, JsonStr);
	return FJsonTool::GetGameSaveDataFormJsonStr(JsonStr);
}



void FFileTool::SaveGame(FGameSaveData GameSaveData, FString RealtivePath, FString FileName)
{
	FString JsonStr = FJsonTool::GetJsonStrFormGameSaveData(GameSaveData);
	SaveStrToFile(JsonStr, RealtivePath, FileName);
}

bool FFileTool::SaveStrToFile(FString InContext, FString RelativePath, FString FileName)
{
	if (!InContext.IsEmpty() && !FileName.IsEmpty())
	{
		//获取绝对路径 例如E:/UE4LearnProjects/InventorySystem04/Saved/AAA/BBB/Test.txt
		FString AbsPath = FPaths::ProjectSavedDir() + RelativePath + FileName;
		//保存字符串
		if (FFileHelper::SaveStringToFile(InContext, *AbsPath))
		{
			return true;
		}
	}
	check(false && "Save File Filed");
	return false;
}

bool FFileTool::LoadFileToStr(FString& OutContext, FString RelativePath, FString FileName)
{
	FString AbsPath = FPaths::ProjectSavedDir() + RelativePath + FileName;
	if (!AbsPath.IsEmpty() && !FileName.IsEmpty())
	{
		if (FPaths::FileExists(AbsPath))
		{
			if (FFileHelper::LoadFileToString(OutContext, *AbsPath))
			{
				return true;
			}
		}
		else
		{
			//当游戏存档文件不存在时,我们这里需要创建一个初始的存档
			//todo...
			int BagGridNum;
			GConfig->GetInt(
				TEXT("BagWidgetInit"),
				TEXT("GridNum"),
				BagGridNum,
				GGameIni
			);

			FGameSaveData GameSaveData;
			GameSaveData.PlayerData.Name = "lisi";
			GameSaveData.PlayerData.Level = 1;
			GameSaveData.PlayerData.Location = FVector(200, 200, 0);



			FBagData BagData;
			BagData.GridNums = BagGridNum;
			BagData.HadItems = { };
			for (int i = 0; i < BagGridNum; i++)
			{
				BagData.GridDatas.Add(FBagGridData(0, 0));
			}

			GameSaveData.PlayerData.BagData = BagData;

			FGroundData GroundData;
			//GroundData.ItemOnGroundDataMap.Add(FName("Item_0"), FItemOnGroundData(FName("Item_0"), 10001, 3, FVector(-300, -200, 200)));
			//GroundData.ItemOnGroundDataMap.Add(FName("Item_1"), FItemOnGroundData(FName("Item_1"), 10002, 1, FVector(-300, -100, 200)));
			GameSaveData.GroundData = GroundData;

			FShortcutsData ShortcutsData;
			int ShortcutsGridNums;
			GConfig->GetInt(
				TEXT("ShortcutsInit"),
				TEXT("ShortcutsGridNums"),
				ShortcutsGridNums,
				GGameIni
			);
			ShortcutsData.ShortcutsGridNums = ShortcutsGridNums;
			for (int i = 0; i < ShortcutsGridNums; i++)
			{
				ShortcutsData.ShortcutsGridDatas.Add(FShortcutsGridData());
			}
			GameSaveData.ShortcutsData = ShortcutsData;

			FEquipmentData EquipmentData;
			for (int i = 0; i < 6; i++)
			{
				FEquipmentGridType1Attr EquipmentGridType1Attr = FDataTableTool::GetEquipmentGridType1Attr(IntToName(i));
				EquipmentData.EquipmentDataMap.Add(EquipmentGridType1Attr.EquipmentGridPart, FEquipmentGridData());
			}
			GameSaveData.EquipmentData = EquipmentData;

			FString JsonStr = FJsonTool::GetJsonStrFormGameSaveData(GameSaveData);
			//UKismetSystemLibrary::PrintString(0, JsonStr);
			SaveStrToFile(JsonStr, RelativePath, FileName);

			OutContext = JsonStr;
			return true;
		}
	}
	return false;
}

