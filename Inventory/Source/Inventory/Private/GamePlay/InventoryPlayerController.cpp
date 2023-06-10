// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/InventoryPlayerController.h"
#include "Inventory/EventManager/EventManager.h"

void AInventoryPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FEventManager::RegistEvent(FName("BagOpened"), this, FName("ToggleMouseFunc"));
}

void AInventoryPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	FEventManager::UnRegistEvent(FName("BagOpened"), this);
}
