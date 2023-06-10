// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ItemOnGroundBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Component/InteractiveComponent.h"
#include "Inventory/EventManager/EventManager.h"
#include "Library/CommonBlueprintFunctionLibrary.h"

// Sets default values
AItemOnGroundBase::AItemOnGroundBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USphereComponent>(FName("Root"));
	SetRootComponent(Root);
	Root->SetSimulatePhysics(true);
	Root->SetSphereRadius(16);
	Root->BodyInstance.bLockXRotation = true;
	Root->BodyInstance.bLockYRotation = true;

	Root->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Root->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Root->SetGenerateOverlapEvents(true);
	Root->SetNotifyRigidBodyCollision(true);//开启模拟物品碰撞事件
	Root->OnComponentHit.AddDynamic(this, &AItemOnGroundBase::OnRootHit);

	ItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMeshComp"));
	ItemMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMeshComp->SetGenerateOverlapEvents(false);
	ItemMeshComp->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ItemMeshComp->SetRelativeLocation(FVector::ZeroVector);

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(FName("WidgetComp"));
	WidgetComp->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	WidgetComp->SetRelativeLocation(FVector::UpVector * 60);
	WidgetComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WidgetComp->SetDrawAtDesiredSize(true);
	WidgetComp->SetVisibility(false);

}

// Called when the game starts or when spawned
void AItemOnGroundBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AItemOnGroundBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemOnGroundBase::Init(FName InIndex)
{
	this->Index = InIndex;
	UpdateDisplay(this->Index);
}

void AItemOnGroundBase::UpdateDisplay(FName InIndex)
{
	FItemOnGroundData ItemOnGroundData = *FFileTool::LoadGame().GroundData.ItemOnGroundDataMap.Find(InIndex);

	this->Id = ItemOnGroundData.ID;
	this->Amount = ItemOnGroundData.Num;

	FItemOnGroundAttr ItemOnGroundAttr = FDataTableTool::GetItemOnGroundAttr(IntToName(ItemOnGroundData.ID));

	ADataAssetMananger::RequestAsyncLoadObject(
		this,
		ItemOnGroundAttr.ItemMesh,
		[this](UObject* Asset)
		{
			ItemMeshComp->SetStaticMesh(Cast<UStaticMesh>(Asset));
		}
	);

}

void AItemOnGroundBase::OnItemBePickUpFinished(FName InIndex)
{
	FEventManager::UnRegistEvent(FName("ItemBePickUpFinishedEvent"), this);

	FItemOnGroundData* ItemOnGroundData = FFileTool::LoadGame().GroundData.ItemOnGroundDataMap.Find(InIndex);

	CurrentInteractiveComponent->EndInteractive();
}

void AItemOnGroundBase::OnRootHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Root->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

void AItemOnGroundBase::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == "Id" || PropertyChangedEvent.GetPropertyName() == "Amount")
	{
		if (UCommonBlueprintFunctionLibrary::IdIsItemCommonAttr(this->Id))
		{
			FItemOnGroundAttr ItemOnGroundAttr = FDataTableTool::GetItemOnGroundAttr(IntToName(this->Id));
			if (this->Amount <= ItemOnGroundAttr.MaxStackNum)
			{
				this->SavedId = this->Id;
				this->SavedAmount = this->Amount;
				struct
				{
					int ID;
					int Amount;
					FVector Location;
				}Params;
				Params.ID = this->Id;
				Params.Amount = this->Amount;
				Params.Location = GetActorLocation();//这个时机无法加载json
				//FEventManager::TriggerEvent(FName("BagItemDropToGroundEvent"), &Params);
			}
		}
		else
		{
			this->Id = this->SavedId;
			this->Amount = this->SavedAmount;
		}
	}
}

void AItemOnGroundBase::Enter(AActor* User)
{
	FItemOnGroundData ItemOnGroundData = *FFileTool::LoadGame().GroundData.ItemOnGroundDataMap.Find(Index);
	FItemOnGroundAttr ItemOnGroundAttr = FDataTableTool::GetItemOnGroundAttr(IntToName(ItemOnGroundData.ID));
	UClass* TipWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemOnGroundAttr.TipBorderWidgetClass);

	if (!IsValid(TipBorderWidget))
	{
		TipBorderWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), TipWidgetClass);

		WidgetComp->SetWidget(TipBorderWidget);


		UFunction* InitFun = TipBorderWidget->FindFunction(FName("Init"));
		if (InitFun)
		{
			struct
			{
				int ID;
				int Amount;
				FName TipContextWidgetClass;
			}Params;
			Params.ID = ItemOnGroundData.ID;
			Params.Amount = ItemOnGroundData.Num;
			Params.TipContextWidgetClass = ItemOnGroundAttr.TipContextWidgetClass;
			TipBorderWidget->ProcessEvent(InitFun, &Params);
		}

	}

	WidgetComp->SetVisibility(true);
	UCommonBlueprintFunctionLibrary::SwitchActorOutLine(this, true);
}

void AItemOnGroundBase::StayTick(AActor* User, float DeletaTime)
{
	FRotator CameraRotation = Cast<APlayerController>(Cast<APawn>(User)->GetController())->PlayerCameraManager->GetCameraRotation();

	WidgetComp->SetWorldRotation((-CameraRotation.Vector()).Rotation());
	ItemMeshComp->AddRelativeRotation(FRotator(0, 120 * DeletaTime, 0));

}

void AItemOnGroundBase::Exit(AActor* User)
{
	//WidgetComp->GetUserWidgetObject()->RemoveFromParent();
	WidgetComp->SetVisibility(false);
	UCommonBlueprintFunctionLibrary::SwitchActorOutLine(this, false);
}

void AItemOnGroundBase::StartInteractive(AActor* User, UInteractiveComponent* InteractiveComponent)
{
	this->CurrentInteractiveComponent = InteractiveComponent;
	if (FFileTool::LoadGame().GroundData.ItemOnGroundDataMap.Find(Index))
	{
		FEventManager::RegistEvent(FName("ItemBePickUpFinishedEvent"), this, FName("OnItemBePickUpFinished"));

		FEventManager::TriggerEvent(FName("ItemBePickUpEvent"), &Index);
	}
}

void AItemOnGroundBase::InteractiveTick(AActor* User, float DeletaTime, UInteractiveComponent* InteractiveComponent)
{
}

void AItemOnGroundBase::EndInteractive(AActor* User)
{
	FItemOnGroundData* ItemOnGroundData = FFileTool::LoadGame().GroundData.ItemOnGroundDataMap.Find(Index);
	if (ItemOnGroundData)
	{
		//还剩下一部分

		Root->AddImpulse(FVector::UpVector * 200, NAME_None, true);
		Root->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	}
	else
	{
		//完全捡起来了 所以ItemOnGtound Actor 没必要存在
		this->Destroy();
	}
}

