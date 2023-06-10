// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/DataTableTool.h"

////静态成员的实现
//UDataTable* UDataTableTool::NavButtonAttr_DT = NULL;
//
////函数的实现
//FNavButtonAttr UDataTableTool::GetNavButtonAttr(FName ButtonName)
//{
//	if (NavButtonAttr_DT == NULL)
//	{
//		NavButtonAttr_DT = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/DataTable/NavButtonAttr.NavButtonAttr'"));
//
//	}
//	//String里面的" "是查找列，前面的ButtonName查找的是行,none是这一行的每一列都可以找到
//	FNavButtonAttr* NavButtonAttr = NavButtonAttr_DT->FindRow<FNavButtonAttr>(ButtonName, FString("None"));
//	check(NavButtonAttr);
//
//	return *NavButtonAttr;
//}

IMPL_DATATABLE(NavButtonAttr, TEXT("DataTable'/Game/DataTable/NavButtonAttr_DT.NavButtonAttr_DT'"));
IMPL_DATATABLE(BagWidgetType1Attr, TEXT("DataTable'/Game/DataTable/BagWidgetType1Attr_DT.BagWidgetType1Attr_DT'"));
IMPL_DATATABLE(ItemOnBagGridAttr, TEXT("DataTable'/Game/DataTable/ItemOnBagGridAttr_DT.ItemOnBagGridAttr_DT'"));
IMPL_DATATABLE(ItemCommonAttr, TEXT("DataTable'/Game/DataTable/ItemCommonAttr_DT.ItemCommonAttr_DT'"));
IMPL_DATATABLE(ItemPotionAttr, TEXT("DataTable'/Game/DataTable/ItemPotionAttr_DT.ItemPotionAttr_DT'"));
IMPL_DATATABLE(ItemOnGroundAttr, TEXT("DataTable'/Game/DataTable/ItemOnGroundAttr_DT.ItemOnGroundAttr_DT'"));
IMPL_DATATABLE(ItemOnShortcutsGridAttr, TEXT("DataTable'/Game/DataTable/ItemOnShortcutsGridAttr_DT.ItemOnShortcutsGridAttr_DT'"));
IMPL_DATATABLE(ShortcutsWidgetType3Attr, TEXT("DataTable'/Game/DataTable/ShortcutsWidgetType3Attr_DT.ShortcutsWidgetType3Attr_DT'"));
IMPL_DATATABLE(EquipmentWidgetType1Attr, TEXT("DataTable'/Game/DataTable/EquipmentWdigetType1Attr_DT.EquipmentWdigetType1Attr_DT'"));
IMPL_DATATABLE(ItemOnEquipmentGridAttr, TEXT("DataTable'/Game/DataTable/ItemOnEquipmentGridAttr_DT.ItemOnEquipmentGridAttr_DT'"));
IMPL_DATATABLE(EquipmentGridType1Attr, TEXT("DataTable'/Game/DataTable/EquipmentGridType1Attr_DT.EquipmentGridType1Attr_DT'"));
//IMPL_DATATABLE(AttributeAttr1, TEXT("DataTable'/Game/DataTable/AttributeAttr1_DT.AttributeAttr1_DT'"));
