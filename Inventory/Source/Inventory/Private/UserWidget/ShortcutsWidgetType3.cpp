// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/ShortcutsWidgetType3.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Library/DataTableTool.h"
#include "Library/FileTool.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"

void UShortcutsWidgetType3::Init(FName ShortcutsType)
{
	
	int ShortcutsGridNums;
	GConfig->GetInt(
		TEXT("ShortcutsInit"),
		TEXT("ShortcutsGridNums"),
		ShortcutsGridNums,
		GGameIni
	);
	//UKismetSystemLibrary::PrintString(0, FString::FromInt(ShortcutsGridNums));
	//FGameSaveData GameSaveData = FFileTool::LoadGame();
	//ShortcutsGridNum = GameSaveData.ShortcutsData.ShortcutsGridNums;

	check(ShortcutsGridNums);
	//UKismetSystemLibrary::PrintString(0, ShortcutsType.ToString());
	FShortcutsWidgetType3Attr ShortcutsWidgetType3Attr = FDataTableTool::GetShortcutsWidgetType3Attr(ShortcutsType);
	UClass* ShortcutsGridWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, ShortcutsWidgetType3Attr.GridType);
	for (int GridIndex = 0; GridIndex < ShortcutsGridNums; GridIndex++)
	{
		UUserWidget* ShortcutsGridWidget = WidgetTree->ConstructWidget<UUserWidget>(ShortcutsGridWidgetClass);
		check(ShortcutsGridWidget);
		ShortcutsHorizontalBox->AddChildToHorizontalBox(ShortcutsGridWidget);

		UFunction* InitFun = ShortcutsGridWidget->FindFunction(FName("Init"));
		if (InitFun)
		{
			ShortcutsGridWidget->ProcessEvent(InitFun, &GridIndex);
		}
	}
}
