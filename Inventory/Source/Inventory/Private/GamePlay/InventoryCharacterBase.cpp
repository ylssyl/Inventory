// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/InventoryCharacterBase.h"
#include "Component/BagComponent.h"
#include "Component/InteractiveComponent.h"
#include "Component/ShortcutsComponent.h"
#include "Component/EquipmentComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/EventManager/EventManager.h"
#include "Inventory/DataAssetMananger/DataAssetMananger.h"
#include "Library/FileTool.h"
#include "Library/DataTableTool.h"
#include "AbilitySystemComponent.h"
#include "GAS/AttributeSetBase.h"

// Sets default values
AInventoryCharacterBase::AInventoryCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BagComponent = CreateDefaultSubobject<UBagComponent>(FName("BagComponent"));
	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(FName("InteractiveComponent"));
	ShortcutsComponent = CreateDefaultSubobject<UShortcutsComponent>(FName("ShortcutsComponent"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(FName("EquipmentComponent"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
	AttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(FName("AttributeSetBase"));

	Hat = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Hat"));
	Jaket = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Jaket"));
	Gloves = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Gloves"));
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Weapon"));
	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Pants"));
	Boots = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Boots"));

	Hat->SetupAttachment(GetMesh());
	Hat->SetMasterPoseComponent(GetMesh());
	Hat->RegisterComponent();
	Hat->bUseAttachParentBound = true;
	Jaket->SetupAttachment(GetMesh());
	Jaket->SetMasterPoseComponent(GetMesh());
	Jaket->RegisterComponent();
	Jaket->bUseAttachParentBound = true;
	Gloves->SetupAttachment(GetMesh());
	Gloves->SetMasterPoseComponent(GetMesh());
	Gloves->RegisterComponent();
	Gloves->bUseAttachParentBound = true;
	Weapon->SetupAttachment(GetMesh());
	Weapon->SetMasterPoseComponent(GetMesh());
	Weapon->RegisterComponent();
	Weapon->bUseAttachParentBound = true;
	Pants->SetupAttachment(GetMesh());
	Pants->SetMasterPoseComponent(GetMesh());
	Pants->RegisterComponent();
	Pants->bUseAttachParentBound = true;
	Boots->SetupAttachment(GetMesh());
	Boots->SetMasterPoseComponent(GetMesh());
	Boots->RegisterComponent();
	Boots->bUseAttachParentBound = true;

}

// Called when the game starts or when spawned
void AInventoryCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	FEventManager::RegistEvent(FName("LoadEquipmentEvent"), this, FName("EquipmentInit"));
	FEventManager::RegistEvent(FName("ClearEquipmentSkeletalEvent"), this, FName("ClearEquipmentSkeletal"));
	EquipmentComponent->EquipInit();

}

void AInventoryCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	FEventManager::UnRegistEvent(FName("LoadEquipmentEvent"), this);
	FEventManager::UnRegistEvent(FName("ClearEquipmentSkeletalEvent"), this);

}

// Called every frame
void AInventoryCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


// Called to bind functionality to input
void AInventoryCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ShortoutsKeys", EInputEvent::IE_Pressed, this, &AInventoryCharacterBase::SelectShortcutsIndex);

}

UAbilitySystemComponent* AInventoryCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AInventoryCharacterBase::GiveAbility()
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


void AInventoryCharacterBase::AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire)
{
	if (AbilitySystemComponent)
	{
		if (HasAuthority() && AbilityToAquire)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToAquire, 1, 0));
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AInventoryCharacterBase::SelectShortcutsIndex(FKey Key)
{
	if (Key == EKeys::NumPadOne)
		NoticeSelectShortcutsBar(0);
	else if (Key == EKeys::NumPadTwo)
		NoticeSelectShortcutsBar(1);
	else if (Key == EKeys::NumPadThree)
		NoticeSelectShortcutsBar(2);
	else if (Key == EKeys::NumPadFour)
		NoticeSelectShortcutsBar(3);
	else return;
}

void AInventoryCharacterBase::NoticeSelectShortcutsBar(int InIndex)
{
	LastShortoutsGridIndex = ShortcutsIndex;
	ShortcutsIndex = InIndex;

	if (LastShortoutsGridIndex != ShortcutsIndex)
	{
		FEventManager::TriggerEvent(FName("StartSelectShortcutsBar"), &ShortcutsIndex);
		FEventManager::TriggerEvent(FName("EndSelectShortcutsBar"), &LastShortoutsGridIndex);
	}


}

void AInventoryCharacterBase::EquipmentInit(int InId)
{
	//加载装备资源
	FItemOnEquipmentGridAttr ItemOnEquipmentGridAttr = FDataTableTool::GetItemOnEquipmentGridAttr(IntToName(InId));
	if (ItemOnEquipmentGridAttr.EquipmentPart == FName("Hat"))
		EquipmentUpdate(Hat, InId);
	else if (ItemOnEquipmentGridAttr.EquipmentPart == FName("Jaket"))
		EquipmentUpdate(Jaket, InId);
	else if (ItemOnEquipmentGridAttr.EquipmentPart == FName("Gloves"))
		EquipmentUpdate(Gloves, InId);
	else if (ItemOnEquipmentGridAttr.EquipmentPart == FName("Pants"))
		EquipmentUpdate(Pants, InId);
	else if (ItemOnEquipmentGridAttr.EquipmentPart == FName("Boots"))
		EquipmentUpdate(Boots, InId);
	else if (ItemOnEquipmentGridAttr.EquipmentPart == FName("Weapon"))
		SyncWeapon(InId);

}

void AInventoryCharacterBase::EquipmentUpdate(USkeletalMeshComponent* InMesh, int InId)
{
	//set skeletalmesh
	SkMesh = InMesh;
	FItemOnEquipmentGridAttr ItemOnEquipmentGridAttr = FDataTableTool::GetItemOnEquipmentGridAttr(IntToName(InId));
	ADataAssetMananger::RequestAsyncLoadObject(
		this,
		ItemOnEquipmentGridAttr.EquipmentSkeletalMesh,
		[this](UObject* Asset) {
			SkMesh->SetSkeletalMesh(Cast<USkeletalMesh>(Asset));
		}
	);
	SyncEquipment(InId);
}

void AInventoryCharacterBase::ClearEquipmentSkeletal(FName InEquipmentPart)
{
	SyncClearEquipment(InEquipmentPart);
	if (InEquipmentPart == FName("Hat"))
		ClearSkeletal(Hat);
	else if (InEquipmentPart == FName("Jaket"))
		ClearSkeletal(Jaket);
	else if (InEquipmentPart == FName("Gloves"))
		ClearSkeletal(Gloves);
	else if (InEquipmentPart == FName("Pants"))
		ClearSkeletal(Pants);
	else if (InEquipmentPart == FName("Boots"))
		ClearSkeletal(Boots);
	else if (InEquipmentPart == FName("Weapon"))
		SyncClearWeapon();

}

void AInventoryCharacterBase::ClearSkeletal(USkeletalMeshComponent* InSkMesh)
{
	SkMesh = InSkMesh;
	SkMesh->SetSkeletalMesh(NULL);
}

void AInventoryCharacterBase::UseShortcutsItem()
{
	if (ShortcutsIndex < 5)
	{
		FEventManager::TriggerEvent(FName("StartUseShortcutsItemEvent"), &ShortcutsIndex);
	}
}



