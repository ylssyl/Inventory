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
	//���뽻������
	virtual void Enter(class AActor* User) = 0;
	//ͣ��ʱ ��������
	virtual void StayTick(class AActor* User, float DeletaTime) = 0;
	//�뿪��������
	virtual void Exit(class AActor* User) = 0;

	//��ʼ�뽻�����󽻻�
	virtual void StartInteractive(class AActor* User, class UInteractiveComponent* InteractiveComponent) = 0;
	//�����뽻�����󽻻�
	virtual void InteractiveTick(class AActor* User, float DeletaTime, class UInteractiveComponent* InteractiveComponent) = 0;
	//�����뽻�����󽻻�
	virtual void EndInteractive(class AActor* User) = 0;
};
