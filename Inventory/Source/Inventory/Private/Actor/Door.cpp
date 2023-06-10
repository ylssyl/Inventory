// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Door.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::Enter(AActor* User)
{
	ReceiveEnter(User);
}

void ADoor::StayTick(AActor* User, float DeletaTime)
{
	ReceiveStayTick(User, DeletaTime);
}

void ADoor::Exit(AActor* User)
{
	ReceiveExit(User);
}

void ADoor::StartInteractive(AActor* User, UInteractiveComponent* InteractiveComponent)
{
	ReceiveStartInteractive(User, InteractiveComponent);
}

void ADoor::InteractiveTick(AActor* User, float DeletaTime, UInteractiveComponent* InteractiveComponent)
{
	ReceiveInteractiveTick(User, DeletaTime, InteractiveComponent);
}

void ADoor::EndInteractive(AActor* User)
{
	ReceiveEndInteractive(User);
}

