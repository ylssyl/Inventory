// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
   GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
   GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
   GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
   GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)



/**
 * 
 */
UCLASS()
class INVENTORY_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSetBase();

	// Health and MaxHealth
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Health);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxHealth);

	FGameplayAttribute HealthAttribute();
	/*UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);*/

	//GAMEPLAYATTRIBUTE_REPNOTIFY
	/*UFUNCTION()
		virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);*/

	//表示数值修改前执行
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue);
	//执行前执行
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data);
	//执行后执行
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	

};
