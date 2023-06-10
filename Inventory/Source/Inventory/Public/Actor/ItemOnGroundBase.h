// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractiveInterface.h"
#include "ItemOnGroundBase.generated.h"

UCLASS()
class INVENTORY_API AItemOnGroundBase : public AActor, public IInteractiveInterface
{
	GENERATED_BODY()

private:
	int SavedId;
	int SavedAmount;

public:
	// Sets default values for this actor's properties
	AItemOnGroundBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* ItemMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWidgetComponent* WidgetComp;


public:
	UFUNCTION()
		void Init(FName InIndex);

	FName Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAttr")
		int Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAttr")
		int Amount;

	UFUNCTION()
		void UpdateDisplay(FName InIndex);
	UFUNCTION()
		void OnItemBePickUpFinished(FName InIndex);

	UFUNCTION()
		void OnRootHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector  NormalImpulse, const FHitResult& Hit);

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

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

	class UUserWidget* TipBorderWidget;
	class UInteractiveComponent* CurrentInteractiveComponent;
};
