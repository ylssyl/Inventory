// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Object/CommonTypes.h"
#include "JsonTool.generated.h"

class FJsonTool
{
	friend class FFileTool;

private:
	//JsonStr -> FGameSaveData
	static FGameSaveData GetGameSaveDataFormJsonStr(FString InJsonStr);
	//FGameSaveData -> JsonStr
	static FString GetJsonStrFormGameSaveData(FGameSaveData GameSaveData);
};

/**
 * 
 */
UCLASS()
class INVENTORY_API UJsonTool : public UObject
{
	GENERATED_BODY()
	
};
