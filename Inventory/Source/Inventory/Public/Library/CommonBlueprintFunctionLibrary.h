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
		/// �����ȡ�ض��������ֶε�ֵ
		/// </summary>
		/// <param name="Obj">��������ָ��</param>
		/// <param name="PropertyName">�ֶ�����</param>
		/// <param name="Value">�ֶ�ֵ�����÷���</param>
		/// <returns>�Ƿ��ҵ��˸��ֶ�</returns>
		static bool GetFloatPropertyOfObject(UObject* Obj, FName PropertyName, float& Value);

	UFUNCTION(BlueprintPure)
		static bool IdIsItemCommonAttr(int InId);

private:
	static UDataTable* ItemOnGroundAttr_DT;
};
