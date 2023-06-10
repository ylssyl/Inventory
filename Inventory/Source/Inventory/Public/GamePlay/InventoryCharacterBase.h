// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "InventoryCharacterBase.generated.h"

//class UGameplayAbility;

UCLASS()
class INVENTORY_API AInventoryCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInventoryCharacterBase();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBagComponent* BagComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UInteractiveComponent* InteractiveComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UEquipmentComponent* EquipmentComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UShortcutsComponent* ShortcutsComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AInventoryCharacterBase")
		class UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AInventoryCharacterBase")
		class UAttributeSetBase* AttributeSetBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AInventoryCharacterBase")
		TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSets")//·ûÎÄ
		TSubclassOf<class UGameplayAbility> Rune;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSets")
		TArray<TSubclassOf<class UAttributeSet>>AttributeSets;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AInventoryCharacterBase")
		class FGameplayTagContainer CharacterTagContainer;*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* Hat;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* Jaket;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* Gloves;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* Pants;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* Boots;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	
	UFUNCTION(BlueprintCallable, Category = "AInventoryCharacterBase")
		void GiveAbility();
	UFUNCTION(BlueprintCallable, Category = "AInventoryCharacterBase")
		void AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire);

	UFUNCTION()
		void SelectShortcutsIndex(FKey Key);
	void NoticeSelectShortcutsBar(int InIndex);
	UFUNCTION(BlueprintCallable)
		void EquipmentInit(int InId);
	UFUNCTION(BlueprintCallable)
		void EquipmentUpdate(USkeletalMeshComponent* SkMesh, int InId);
	UFUNCTION(BlueprintCallable)
		void ClearEquipmentSkeletal(FName InEquipmentPart);
	UFUNCTION(BlueprintCallable)
		void ClearSkeletal(USkeletalMeshComponent* InSkMesh);
	
	
	
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SyncEquipment(int InId);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SyncWeapon(int InId);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SyncClearEquipment(FName InEquipmentPart);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SyncClearWeapon();
	UFUNCTION(BlueprintCallable)
		void UseShortcutsItem();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USkeletalMeshComponent* SkMesh = nullptr;
private:
	int LastShortoutsGridIndex = 100;
	int ShortcutsIndex = 99;

};
