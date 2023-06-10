// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/InteractiveInterface.h"
#include "InteractiveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORY_API UInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractiveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		bool StartInteractive(class AActor*& OutInteractiveActor);

	UFUNCTION(BlueprintCallable)
		class AActor* EndInteractive();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TraceRange = 150;

	virtual IInteractiveInterface* Rule(TArray<class AActor*> InteractiveActors);

	IInteractiveInterface* Last;//上一帧可以交互的对象
	IInteractiveInterface* Current;//当前可以交互的对象
	IInteractiveInterface* Interactiving;//正在交互的对象
		
};
