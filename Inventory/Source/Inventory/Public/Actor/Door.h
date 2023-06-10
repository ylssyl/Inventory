// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractiveInterface.h"
#include "Door.generated.h"

UCLASS()
class INVENTORY_API ADoor : public AActor, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region IInteractiveInterface Impl
	//进入交互对象
	virtual void Enter(class AActor* User)override;
	//停留时 交互对象
	virtual void StayTick(class AActor* User, float DeletaTime)override;
	//离开交互对象
	virtual void Exit(class AActor* User)override;

	//开始与交互对象交互
	virtual void StartInteractive(class AActor* User, class UInteractiveComponent* InteractiveComponent)override;
	//正在与交互对象交互
	virtual void InteractiveTick(class AActor* User, float DeletaTime, class UInteractiveComponent* InteractiveComponent)override;
	//结束与交互对象交互
	virtual void EndInteractive(class AActor* User)override;
#pragma endregion

	//纯虚函数  只不过只能由子类蓝图来实现
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Enter"))
		void ReceiveEnter(class AActor* User);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StayTick"))
		void ReceiveStayTick(class AActor* User, float DeletaTime);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Exit"))
		void ReceiveExit(class AActor* User);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StartInteractive"))
		void ReceiveStartInteractive(class AActor* User, class UInteractiveComponent* InteractiveComponent);
	//正在与交互对象交互
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "InteractiveTick"))
		void ReceiveInteractiveTick(class AActor* User, float DeletaTime, class UInteractiveComponent* InteractiveComponent);
	//结束与交互对象交互
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "EndInteractive"))
		void ReceiveEndInteractive(class AActor* User);

};
