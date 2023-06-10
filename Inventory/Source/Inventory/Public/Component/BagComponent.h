// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BagComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORY_API UBagComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBagComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void SortItem();

	UFUNCTION(BlueprintCallable)
		int AddItem(int ID, int Amount);
	UFUNCTION(BlueprintCallable)
		void OnPickItem(FName ItemIndex);

	UFUNCTION()
		void BagGridDragToBagGrid(int FromIndex, int ToIndex);
	UFUNCTION()
		void ClearBagGridByIndex(int Index);
	UFUNCTION()
		void ShortcutsGridDragToBagGrid(int FromIndex, int ToIndex);
	UFUNCTION()
		void BagGridDragToEquipmentGrid(int FromIndex, int ToIndex);

	//最接近最大叠加数但不等于的格子的索引
	int FindFreeGridIndex(int ID);
	//寻找空格子
	int FindEmptyGridIndex();

	UFUNCTION()
		void StartCheckBagItem(int InGridIndex);
	UFUNCTION()
		void EndCheckBagItem(int InGridIndex);
};
