// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "GAS/AttributeSetBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
	AttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(FName("AttributeSetBase"));


}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::GiveAbility()
{
	if (AbilitySystemComponent)
	{
		if (HasAuthority() && CharacterAbilities.Num())
		{
			for (int i = 0; i < CharacterAbilities.Num(); i++)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(CharacterAbilities[i]));
			}
			AbilitySystemComponent->InitAbilityActorInfo(this, this);
		}
	}
}

void ACharacterBase::OnHealthChanged(float Health, float MaxHealth)
{

	if (Health <= 0.0f)
	{
		BP_Die();
	}

	BP_OnHealthChanged(Health, MaxHealth);
}


