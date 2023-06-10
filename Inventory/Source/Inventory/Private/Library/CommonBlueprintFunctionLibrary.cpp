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

	// ͨ�� UClass ָ�빹���ֶε��������󣬲����е���ѭ��
	for (TFieldIterator<FFloatProperty> It(ObjClass); It; ++It)
	{
		// �����õ������õ�FPropertyָ��
		auto Property = *It;
		// �ж��ֶε������Ƿ�ƥ��
		if (Property->GetName() == PropertyName.ToString())
		{
			// ͨ��API��������������ָ��ƫ�Ƶ���Ӧ�ֶ�λ�ã��õ�ָ���ֶε�ָ��
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
