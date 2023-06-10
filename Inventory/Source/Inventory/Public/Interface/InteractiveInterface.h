// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IInteractiveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//进入交互对象
	virtual void Enter(class AActor* User) = 0;
	//停留时 交互对象
	virtual void StayTick(class AActor* User, float DeletaTime) = 0;
	//离开交互对象
	virtual void Exit(class AActor* User) = 0;

	//开始与交互对象交互
	virtual void StartInteractive(class AActor* User, class UInteractiveComponent* InteractiveComponent) = 0;
	//正在与交互对象交互
	virtual void InteractiveTick(class AActor* User, float DeletaTime, class UInteractiveComponent* InteractiveComponent) = 0;
	//结束与交互对象交互
	virtual void EndInteractive(class AActor* User) = 0;
};
