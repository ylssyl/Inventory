// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/TipContextWidgetType1.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Library/DataTableTool.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"

void UTipContextWidgetType1::Init(int ID)
{
	DesText->SetText(FText::FromName(FDataTableTool::GetItemCommonAttr(IntToName(ID)).Describe));
	FItemPotionAttr ItemPotionAttr = FDataTableTool::GetItemPotionAttr(IntToName(ID));

	UClass* PropertyEffectInfoWidgetClass = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/UMG/PotionEffectInfoWidget_WBP.PotionEffectInfoWidget_WBP_C'"));

	for (TPair<FName, FPropertyEffect> KeyValuePair : ItemPotionAttr.AddPropertyNames)
	{
		UUserWidget* PropertyEffectInfoWidget = WidgetTree->ConstructWidget<UUserWidget>(PropertyEffectInfoWidgetClass);
		PropertyBox->AddChild(PropertyEffectInfoWidget);

		UFunction* InitFun = PropertyEffectInfoWidget->FindFunction(FName("Init"));
		if (InitFun)
		{
			struct
			{
				FName PropertyName;
				FPropertyEffect PropertyEffect;
			}Params;
			
			Params.PropertyName = KeyValuePair.Key;
			Params.PropertyEffect = KeyValuePair.Value;

			PropertyEffectInfoWidget->ProcessEvent(InitFun, &Params);
		}
	}
}

