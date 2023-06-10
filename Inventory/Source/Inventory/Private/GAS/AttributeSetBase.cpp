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

	//���������޸ĵ�������Health���ԣ���ô������Clamp
	if (Attribute == HealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
		//Clamp��ʾ��ȡ��ĳ����Χ�ڣ�����������Ϥ���뿴�ٷ��ĵ���FMath�й������Ľ���

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

	//���ʩ�Ŵ˴��޸ĵ�Դͷ
	UAbilitySystemComponent* Source = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	//�˴��޸ĵ�������Health��
	if (HealthAttribute() == Data.EvaluatedData.Attribute)
	{
		AActor* TargetActor = nullptr;
		AController* TargetController = nullptr;
		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();

			//�޸�Ŀ���TargetActor��ץȡ����
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

			//�����޸ĵ�ԴͷSourceActor��ץȡ����
			UE_LOG(LogTemp, Warning, TEXT("The SourceActor Name is:%s"), *(SourceActor->GetName()));
		}

		//Clamp����ʵClamp���ǡ�ִ�к��¼�������Ҫ���߼�
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, 100.f));

		//����UI
		//��ִ�к��¼�������Ҫ���߼��ǣ������ֵ̫�ͣ���ô���ִ����ض����飬��Health���㣬��ô������
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
