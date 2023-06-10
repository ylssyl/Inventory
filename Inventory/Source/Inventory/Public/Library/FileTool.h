// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Object/CommonTypes.h"
#include "FileTool.generated.h"

class FFileTool 
{
public:
	static void ClearPlayerBagGridByIndex(int Index);
	static void ClearPlayerShortoutsGridByIndex(int Index);
	static void ClearPlayerEquipmentGridByIndex(int Index);

	static void SwapShortoutsGridDataAndBagGridData(FBagGridData &InBagGridData, FShortcutsGridData &InShortcutsGridData);
	static void SwapEquipmentGridDataAndBagGridData(FBagGridData &InBagGridData, FEquipmentGridData &InEquipmentGridData);

	static bool SetItemOnGroundDataNumByIndex(FName InIndex, int NewNum);

	static TArray<FBagGridData> GetPlayerBagGridDatas();
	static void SetPlayerBagGridDatas(TArray<FBagGridData> NewBagGridDatas);

	static bool IsHadItem(int ID);
	static void AddHadItem(int ID);

	static	FBagGridData GetPlayerBagGridDataByIndex(int Index);
	static void SetPlayerBagGridDataByIndex(int FoundIndex, FBagGridData NewBagGridData);
	
	static	FShortcutsGridData GetShortcutsGridDataByIndex(int Index);
	static void SetShortcutsGridDataByIndex(int FoundIndex, FShortcutsGridData NewShortcutsGridData);

	static	FEquipmentGridData GetEquipmentGridDataByIndex(int Index);
	static void SetEquipmentGridDataByIndex(int FoundIndex, FEquipmentGridData NewShortcutsGridData);

	static FGameSaveData  LoadGame(FString RealtivePath = FString("/SaveDatas"), FString FileName = FString("/Save00.json"));
	static void SaveGame(FGameSaveData GameSaveData, FString RealtivePath = FString("/SaveDatas"), FString FileName = FString("/Save00.json"));

private:
	static bool SaveStrToFile(FString InContext, FString RelativePath, FString FileName);
	static bool LoadFileToStr(FString& OutContext, FString RelativePath, FString FileName);
};

/**
 * 
 */
UCLASS()
class INVENTORY_API UFileTool : public UObject
{
	GENERATED_BODY()
	
};
