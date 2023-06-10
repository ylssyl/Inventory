// Fill out your copyright notice in the Description page of Project Settings.


#include "Chair.h"
#include "Library/DataTableTool.h"
#include "Kismet/GameplayStatics.h"
#include "Library/FileTool.h"
#include "Library/JsonTool.h"

AChair::AChair()
{
	//只能用于构造函数
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChairMeshFinder(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'"));
	check(ChairMeshFinder.Succeeded());
	GetStaticMeshComponent()->SetStaticMesh(ChairMeshFinder.Object);
}

void AChair::BeginPlay()
{
	Super::BeginPlay();
	//不能用于构造函数，其他的函数都能用
	/*UStaticMesh* ChairMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'"));
	check(ChairMesh);
	GetStaticMeshComponent()->SetStaticMesh(ChairMesh);*/
	//两种都是同步加载，加载后运行后面代码
	//文件小和少建议用异步加载

	//FNavButtonAttr NavButtonAttr = FDataTableTool::GetNavButtonAttr(FName("ToggleBagButton"));

	/*bool Result = FFileTool::SaveStrToFile("abc123", "/AAA/", "Test.txt");
	FString OutStr;
	FFileTool::LoadFileToStr(OutStr, "/AAA/", "Test.txt");
	if (Result)
		UKismetSystemLibrary::PrintString(0, "Success");
		UKismetSystemLibrary::PrintString(0, *OutStr);*/

	/*FGameSaveData GameSaveData;
	GameSaveData.PlayerData.Name = "hyy";
	GameSaveData.PlayerData.Level = 1;
	GameSaveData.PlayerData.Location = FVector(200, 200, 0);

	FBagData BagData;
	BagData.GridNums = 3;
	BagData.HadItems = { 10002, 10003};
	BagData.GridDatas = { FBagGridData(10001, 3), FBagGridData(), FBagGridData(10002, 2) };
	GameSaveData.PlayerData.BagData = BagData;*/

	//FString JsonStr = FJsonTool::GetJsonStrFormGameSaveData(GameSaveData);

	/*FString JsonStr;
	FFileTool::LoadFileToStr(JsonStr, FString("/SaveDatas"), FString("/Save00.json"));
	FGameSaveData GameSaveData = FJsonTool::GetGameSaveDataFormJsonStr(JsonStr);
	PrintGameSaveData(GameSaveData);*/

	//FFileTool::SaveGame(GameSaveData);
	FGameSaveData GameSaveData = FFileTool::LoadGame();
	//UKismetSystemLibrary::PrintString(0, FString::FromInt(GameSaveData.PlayerData.Level));
	PrintGameSaveData(GameSaveData);
}
