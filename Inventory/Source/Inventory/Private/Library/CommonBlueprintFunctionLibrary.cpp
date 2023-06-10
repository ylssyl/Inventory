// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/CommonBlueprintFunctionLibrary.h"

UDataTable* UCommonBlueprintFunctionLibrary::ItemOnGroundAttr_DT = NULL;

void UCommonBlueprintFunctionLibrary::SwitchActorOutLine(AActor* Actor, bool On, int StencilValue)
{
	auto AllMeshes = Actor->GetComponentsByClass(UMeshComponent::StaticClass());

	for (auto Mesh : AllMeshes)
	{
		auto MeshComp = Cast<UMeshComponent>(Mesh);
		MeshComp->SetRenderCustomDepth(On);
		MeshComp->SetCustomDepthStencilValue(StencilValue);
	}


}

bool UCommonBlueprintFunctionLibrary::GetFloatPropertyOfObject(UObject* Obj, FName PropertyName, float& Value)
{
	Value = 0;
	auto ObjClass = Obj->GetClass();

	// 通过 UClass 指针构造字段迭代器对象，并进行迭代循环
	for (TFieldIterator<FFloatProperty> It(ObjClass); It; ++It)
	{
		// 解引用迭代器得到FProperty指针
		auto Property = *It;
		// 判断字段的名字是否匹配
		if (Property->GetName() == PropertyName.ToString())
		{
			// 通过API，将对象容器的指针偏移到相应字段位置，得到指向字段的指针
			auto ValuePtr = Property->ContainerPtrToValuePtr<float>(Obj);
			Value = *ValuePtr;
			return true;
		}
	}

	return false;
}


bool UCommonBlueprintFunctionLibrary::IdIsItemCommonAttr(int InId)
{
	if (ItemOnGroundAttr_DT == NULL)
	{
		ItemOnGroundAttr_DT = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/DataTable/ItemOnGroundAttr_DT.ItemOnGroundAttr_DT'"));

	}
	FItemOnGroundAttr* ItemOnGroundAttr = ItemOnGroundAttr_DT->FindRow<FItemOnGroundAttr>(IntToName(InId), FString("None"));
	if (ItemOnGroundAttr)
	{
		return true;
	}
	return false;
}
