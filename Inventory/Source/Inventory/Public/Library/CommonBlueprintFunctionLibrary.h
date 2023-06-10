// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Library/DataTableTool.h"
#include "CommonBlueprintFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class INVENTORY_API UCommonBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static void SwitchActorOutLine(AActor* Actor, bool On, int StencilValue = 1);

	UFUNCTION(BlueprintPure)
		/// <summary>
		/// 反射获取特定浮点数字段的值
		/// </summary>
		/// <param name="Obj">容器对象指针</param>
		/// <param name="PropertyName">字段名称</param>
		/// <param name="Value">字段值的引用返回</param>
		/// <returns>是否找到了该字段</returns>
		static bool GetFloatPropertyOfObject(UObject* Obj, FName PropertyName, float& Value);

	UFUNCTION(BlueprintPure)
		static bool IdIsItemCommonAttr(int InId);

private:
	static UDataTable* ItemOnGroundAttr_DT;
};
