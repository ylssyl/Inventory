// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InteractiveComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInteractiveComponent::UInteractiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	TArray<AActor*> InteractiveActors;
	TArray<FHitResult> OutHitResults;
	if (UKismetSystemLibrary::SphereTraceMulti(
		this,
		this->GetOwner()->GetActorLocation(),
		this->GetOwner()->GetActorLocation() + FVector(0, 0, 1),
		TraceRange,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		true,
		{ this->GetOwner() },
		EDrawDebugTrace::None,
		OutHitResults,
		true
	))
	{
		for (FHitResult OutHitResult : OutHitResults)
		{
			IInteractiveInterface* InteractiveInterface = Cast<IInteractiveInterface>(OutHitResult.GetActor());
			if (InteractiveInterface)
			{
				InteractiveActors.AddUnique(OutHitResult.GetActor());
			}

		}

	}
	//ɸѡ����
	Current = Rule(InteractiveActors);

	if (Current != nullptr)
	{
		Current->StayTick(this->GetOwner(), DeltaTime);
	}
	if (Interactiving != nullptr)
	{
		Interactiving->InteractiveTick(this->GetOwner(), DeltaTime, this);
	}
	//˵����һ֡û�м�⵽���Խ����Ķ���,��ǰ֡��⵽
	if (Last == nullptr && Current != nullptr)
	{
		Current->Enter(GetOwner());
		Last = Current;
		return;
	}
	//˵����һ֡��⵽��  ��ǰ��ⲻ��
	if (Last != nullptr && Current == nullptr)
	{
		Last->Exit(GetOwner());
		Last = Current;
		return;
	}

	//˵����һ֡��⵽�˶���  ��ǰ֡Ҳ��⵽�˶���  ��  ��һ֡���ĺ͵�ǰ���Ĳ�ͬ
	if (Last != nullptr && Current != nullptr && Last != Current)
	{
		Last->Exit(GetOwner());
		Current->Enter(GetOwner());
		Last = Current;
		return;
	}

}

bool UInteractiveComponent::StartInteractive(AActor*& OutInteractiveActor)
{
	if (Current != 0 && Interactiving == nullptr)
	{
		Interactiving = Current;
		Interactiving->StartInteractive(GetOwner(), this);
		
		OutInteractiveActor = Cast<AActor>(Interactiving);
		return true;
	}

	Interactiving = nullptr;
	return false;
}

AActor* UInteractiveComponent::EndInteractive()
{
	if (Interactiving != nullptr)
	{
		Interactiving->EndInteractive(GetOwner());
		AActor* Temp = Cast<AActor>(Interactiving);
		Interactiving = nullptr;
		return Temp;
	}

	return nullptr;
}

IInteractiveInterface* UInteractiveComponent::Rule(TArray<AActor*> InteractiveActors)
{
	IInteractiveInterface* InteractiveInterface = nullptr;
	float MinDistanceSqrt = 999999;

	for (AActor* InteractiveActor : InteractiveActors)
	{
		float DistanceSqrt = FVector::DistSquared(GetOwner()->GetActorLocation(), InteractiveActor->GetActorLocation());
		if (MinDistanceSqrt > DistanceSqrt)
		{
			MinDistanceSqrt = DistanceSqrt;
			InteractiveInterface = Cast<IInteractiveInterface>(InteractiveActor);
		}

	}

	return InteractiveInterface;
}

