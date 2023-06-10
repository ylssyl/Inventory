// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"


UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORY_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION()
		int Query(int InEquipID);
	UFUNCTION(BlueprintCallable)
		bool AddEquip(int ID, int Amount);
	UFUNCTION(BlueprintCallable)
		void EquipUpdate(int InId);
	UFUNCTION(BlueprintCallable)
		void EquipUpdate1(int InId, int InIndex);
	UFUNCTION(BlueprintCallable)
		void EquipInit();
	FName GetEquipmentPart(int InIndex);
		
};
