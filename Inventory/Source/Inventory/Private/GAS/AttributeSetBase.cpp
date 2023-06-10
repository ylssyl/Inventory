// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Kismet/GameplayStatics.h"

UAttributeSetBase::UAttributeSetBase()
{

}

//void UAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
//{
//	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Health, OldHealth);
//}

//void UAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
//{
//	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxHealth, OldMaxHealth);
//}

FGameplayAttribute UAttributeSetBase::HealthAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(),
		GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health));
	return FGameplayAttribute(Property);
}

void UAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("PreAttributeChagne UAttributeSetBase"));

	//如果这个待修改的属性是Health属性，那么就这样Clamp
	if (Attribute == HealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
		//Clamp表示截取在某个范围内，如果这个不熟悉，请看官方文档中FMath中关于它的介绍

		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("UAttributeSetBase"));
	}
}

bool UAttributeSetBase::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return true;
}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("This AS is ready to be changed"));

	//获得施放此次修改的源头
	UAbilitySystemComponent* Source = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	//此次修改的属性是Health吗？
	if (HealthAttribute() == Data.EvaluatedData.Attribute)
	{
		AActor* TargetActor = nullptr;
		AController* TargetController = nullptr;
		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();

			//修改目标的TargetActor的抓取方法
			UE_LOG(LogTemp, Warning, TEXT("The TargetActor Name is:%s"), *(TargetActor->GetName()));
		}

		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		AController* SourcePlayerController = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid()
			&& Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();

			//发起修改的源头SourceActor的抓取方法
			UE_LOG(LogTemp, Warning, TEXT("The SourceActor Name is:%s"), *(SourceActor->GetName()));
		}

		//Clamp，其实Clamp不是“执行后事件”最重要的逻辑
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, 100.f));

		//更新UI
		//“执行后事件”最重要的逻辑是：如果数值太低，那么表现触发特定事情，如Health触零，那么死亡。
		UKismetSystemLibrary::PrintString(0, FString::FromInt(Health.GetCurrentValue()));
		UKismetSystemLibrary::PrintString(0, FString::FromInt(MaxHealth.GetCurrentValue()));
		UFunction* Func = TargetActor->FindFunction(FName("OnHealthChanged"));
		if (Func)
		{
			struct
			{
				float InHealth;
				float InMaxHealth;
			}Params;
			Params.InHealth = Health.GetCurrentValue();
			Params.InMaxHealth = MaxHealth.GetCurrentValue();
			TargetActor->ProcessEvent(Func, &Params);
		}


	}
	return;
}
