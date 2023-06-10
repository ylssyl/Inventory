// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Object/CommonTypes.h"
#include "Engine/DataTable.h"
#include "DataTableTool.generated.h"

#define IntToName(IntParam) (FName(*FString::FromInt(IntParam)))

#define SIGNED_DATATABLE(AttrType) \
private:\
static UDataTable* ##AttrType##_DT;\
public:\
	UFUNCTION(BlueprintPure)\
		static F##AttrType Get##AttrType##(FName RowName);


#define IMPL_DATATABLE(AttrType,TablePath) \
UDataTable* FDataTableTool::##AttrType##_DT = NULL;\
F##AttrType FDataTableTool::Get##AttrType##(FName RowName)\
{\
	if (!##AttrType##_DT)\
	{\
		##AttrType##_DT = LoadObject<UDataTable>(NULL, TablePath);\
		check(##AttrType##_DT);\
	}\
	F##AttrType * Attr = ##AttrType##_DT->FindRow<F##AttrType>(RowName, FString("None"));\
	check(Attr);\
	return *Attr;\
}

class FDataTableTool
{
	SIGNED_DATATABLE(NavButtonAttr);
	SIGNED_DATATABLE(BagWidgetType1Attr);
	SIGNED_DATATABLE(ItemOnBagGridAttr);
	SIGNED_DATATABLE(ItemCommonAttr);
	SIGNED_DATATABLE(ItemPotionAttr);
	SIGNED_DATATABLE(ItemOnGroundAttr);
	SIGNED_DATATABLE(ItemOnShortcutsGridAttr);
	SIGNED_DATATABLE(ShortcutsWidgetType3Attr);
	SIGNED_DATATABLE(EquipmentWidgetType1Attr);
	SIGNED_DATATABLE(ItemOnEquipmentGridAttr);
	SIGNED_DATATABLE(EquipmentGridType1Attr);
	//SIGNED_DATATABLE(AttributeAttr1);

};

/**
 * 
 */
UCLASS()
class INVENTORY_API UDataTableTool : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//public:
//	UFUNCTION(BlueprintPure)
//		static FNavButtonAttr GetNavButtonAttr(FName ButtonName);
//
//private:
//	static UDataTable* NavButtonAttr_DT;
	
};
