// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EventManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FMultCastDelegate,void**);

//����
class FEventManager {
private:
	static TSharedPtr<FEventManager> Get() {
		if (!Instance.IsValid())
			Instance = MakeShareable(new FEventManager);
		return  Instance;
	}
	static TSharedPtr<FEventManager> Instance;

	TMap<FName, TArray<FScriptDelegate>> Events;

	FMultCastDelegate MultCastDelegate;


public:
	template<typename UserClass>
	static void AddUObjectFun(
		UserClass* UserObj, 
		typename TBaseDelegate<void, void**>   //void** ����(��void*[])������
		::TUObjectMethodDelegate<UserClass>    //::TRawMethodDelegate<UserClass>
		::FMethodPtr FunPtr
	)
	{
		Get()->MultCastDelegate.AddUObject(UserObj, FunPtr);
	};

	template<typename UserClass>
	static void UnbindFun(UserClass* UserObj) {
		Get()->MultCastDelegate.RemoveAll(UserObj);
	};
	

	static void ExecuteDelegate(void** Params) {
		Get()->MultCastDelegate.Broadcast(Params);
	};


	static void RegistEvent(FName EventName, UObject* Obj, FName FunName);
	static void UnRegistEvent(FName EventName, UObject* Obj);
	static void TriggerEvent(FName EventName, void* Parameters);
};


/*
void APC::BeginPlay()
{
	Super::BeginPlay();

	//��
	FEventManager::AddUObjectFun(this, &APC::Test2);
	//FEventManager::UnbindFun();

	//���ò���
	int Num = 7;
	bool BoolValue = true;
	FVector Location = FVector::ForwardVector;
	void* Params[] = {&Num,&BoolValue,&Location};
	//ί�еĵ���
	FEventManager::ExecuteDelegate(Params);

}

void APC::Test2(void* Params[])
{
	int NumValue = *(int*)(Params[0]);
	UE_LOG(LogTemp, Warning, TEXT("*(int*)(Params[0]) == %d"), NumValue);
	bool BoolValue = *(bool*)(Params[1]);
	UE_LOG(LogTemp, Warning, TEXT("*(bool*)(Params[1]) == %d"), BoolValue);
	FVector Location = *(FVector*)(Params[2]);
	UE_LOG(LogTemp, Warning, TEXT("*(FVector*)(Params[2]) == %s"), *Location.ToString());
}


*/


/**
 * 
 */
UCLASS()
class  UEventManager : public UObject
{
	GENERATED_BODY()
public:


};
