// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CommonTypes.generated.h"


USTRUCT(Blueprintable, BlueprintType)
struct FEquipmentGridType1Attr :public FTableRowBase {
	GENERATED_BODY()
public:
	FEquipmentGridType1Attr() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName EquipmentGridPart;//װ�����ӵĲ���

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector2D Size;//���Ӵ�С 

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector2D Location;//��UI�ϵ�����

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName BGIcon;  //����ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName BorderIcon;  //�߿�ͼƬ

};

USTRUCT(Blueprintable, BlueprintType)
struct FEquipmentGridData
{
	GENERATED_BODY()
public:
	FEquipmentGridData() :EquipmentGridDataID(0), EquipmentGridDataNum(0) {};
	FEquipmentGridData(int InID, int InNum) :EquipmentGridDataID(InID), EquipmentGridDataNum(InNum) {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int EquipmentGridDataID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int EquipmentGridDataNum;
};

USTRUCT(Blueprintable, BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()
public:
	FEquipmentData() {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TMap<FName, FEquipmentGridData> EquipmentDataMap;
};


USTRUCT(Blueprintable, BlueprintType)
struct FShortcutsGridData
{
	GENERATED_BODY()
public:
	FShortcutsGridData() :ShortcutsGridDataID(0), ShortcutsGridDataNum(0) {};
	FShortcutsGridData(int InID, int InNum) :ShortcutsGridDataID(InID), ShortcutsGridDataNum(InNum) {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ShortcutsGridDataID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ShortcutsGridDataNum;
};

USTRUCT(Blueprintable, BlueprintType)
struct FShortcutsData
{
	GENERATED_BODY()
public:
	FShortcutsData() {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ShortcutsGridNums;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FShortcutsGridData> ShortcutsGridDatas;
};

USTRUCT(Blueprintable, BlueprintType)
struct FBagGridData
{
	GENERATED_BODY()
public:
	FBagGridData() :ID(0), Num(0) {};
	FBagGridData(int ID, int Num) :ID(ID), Num(Num) {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Num;
};

USTRUCT(Blueprintable, BlueprintType)
struct FBagData {
	GENERATED_BODY()
public:
	FBagData() {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int GridNums;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int> HadItems;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FBagGridData> GridDatas;
};

USTRUCT(Blueprintable, BlueprintType)
struct FPlayerData {
	GENERATED_BODY()
public:
	FPlayerData() {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FBagData BagData;
};

USTRUCT(Blueprintable, BlueprintType)
struct FItemOnGroundData {
	GENERATED_BODY()
public:
	FItemOnGroundData() {};
	FItemOnGroundData(FName Index, int ID, int Num, FVector Location) :Index(Index), ID(ID), Num(Num), Location(Location) {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName Index;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Num;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector Location;
};

USTRUCT(Blueprintable, BlueprintType)
struct FGroundData {
	GENERATED_BODY()
public:
	FGroundData() {};

	//���ϵ������Ʒ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TMap<FName, FItemOnGroundData> ItemOnGroundDataMap;
};

USTRUCT(Blueprintable, BlueprintType)
struct FGameSaveData {
	GENERATED_BODY()
public:
	FGameSaveData() {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FPlayerData PlayerData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FGroundData GroundData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FShortcutsData ShortcutsData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FEquipmentData EquipmentData;
};




USTRUCT(Blueprintable, BlueprintType)
struct FItemWeaponAttr :public FTableRowBase {
	GENERATED_BODY()
public:
	FItemWeaponAttr() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Attack;

};

USTRUCT(Blueprintable, BlueprintType)
struct FPropertyEffect {
	GENERATED_BODY()
public:
	FPropertyEffect() {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Amount;   //������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Duration;   //���Ϊ0  �����ǵ�˲������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float EffectDuration;   //���Ϊ0  ��������  ���Ϊ3   


};

USTRUCT(Blueprintable, BlueprintType)
struct FAttributeAttr1 {
	GENERATED_BODY()
public:
	FAttributeAttr1() {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BaseValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MinValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName DerivedAttributeInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCanStack;

};


USTRUCT(Blueprintable, BlueprintType)
struct FItemPotionAttr :public FTableRowBase {
	GENERATED_BODY()
public:
	FItemPotionAttr() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TMap<FName, FPropertyEffect> AddPropertyNames;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Cost; //����
};


USTRUCT(Blueprintable, BlueprintType)
struct FItemCommonAttr :public FTableRowBase {
	GENERATED_BODY()
public:
	FItemCommonAttr() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName ItemName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName Describe;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int BugPrice;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int SellPrice;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName UsedItem;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName ItemMesh;
};


USTRUCT(Blueprintable, BlueprintType)
struct FItemOnGroundAttr :public FTableRowBase {
	GENERATED_BODY()
public:
	FItemOnGroundAttr() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName ItemMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int MaxStackNum;//���ѵ��� 

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TipBorderWidgetClass;  //��ʾ��ı߿�UI

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TipContextWidgetClass;  //��ʾ�������UI

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName ItemClass;
};



USTRUCT(Blueprintable, BlueprintType)
struct FItemOnBagGridAttr :public FTableRowBase {
	GENERATED_BODY()
public:
	FItemOnBagGridAttr() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector2D Size;//���Ӵ�С 80*80

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName BGIcon;  //����ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName BorderIcon;  //�߿�ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName ItemIcon; //����ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int MaxStackNum;//���ѵ���

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TipBorderWidgetClass;  //��ʾ��ı߿�UI

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TipContextWidgetClass;  //��ʾ�������UI


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName DragWidgetClass;   //��ק����ʽUI

};


USTRUCT(Blueprintable, BlueprintType)
struct FItemOnShortcutsGridAttr :public FTableRowBase {
	GENERATED_BODY()
public:
	FItemOnShortcutsGridAttr() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector2D Size;//���Ӵ�С 80*80

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName BGIcon;  //����ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName BorderIcon;  //�߿�ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName ItemIcon; //����ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int MaxStackNum;//���ѵ���

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TipBorderWidgetClass;  //��ʾ��ı߿�UI

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TipContextWidgetClass;  //��ʾ�������UI


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName DragWidgetClass;   //��ק����ʽUI

};

USTRUCT(Blueprintable, BlueprintType)
struct FItemOnEquipmentGridAttr :public FTableRowBase {
	GENERATED_BODY()
public:
	FItemOnEquipmentGridAttr() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector2D Size;//���Ӵ�С 80*80

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName BGIcon;  //����ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName BorderIcon;  //�߿�ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName ItemIcon; //����ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int MaxStackNum;//���ѵ���

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TipBorderWidgetClass;  //��ʾ��ı߿�UI

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TipContextWidgetClass;  //��ʾ�������UI


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName DragWidgetClass;   //��ק����ʽUI
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName EquipmentPart;     //װ����λ
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName EquipmentSkeletalMesh;

};


//������ʽ1������
USTRUCT(Blueprintable, BlueprintType)
struct FBagWidgetType1Attr :public FTableRowBase {
	GENERATED_BODY()
public:
	FBagWidgetType1Attr() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TitleName;//����������
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TitleTexture;//������ͼƬ
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName BagBGTexture;//����ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName CloseButtonNormalIcon;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName CloseButtonHoverIcon;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName CloseButtonPressedIcon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector2D BagSize;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName GridType;

};

USTRUCT(Blueprintable, BlueprintType)
struct FShortcutsWidgetType3Attr :public FTableRowBase {
	GENERATED_BODY()
public:
	FShortcutsWidgetType3Attr() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TitleTexture;//������ͼƬ
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName BagBGTexture;//����ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector2D BagSize;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName GridType;

};

USTRUCT(Blueprintable, BlueprintType)
struct FEquipmentWidgetType1Attr :public FTableRowBase {
	GENERATED_BODY()
public:
	FEquipmentWidgetType1Attr() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TitleTexture;//������ͼƬ
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName BagBGTexture;//����ͼƬ

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector2D BagSize;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName EquipmentGridType;

};


//������ť����
USTRUCT(Blueprintable, BlueprintType)
struct FNavButtonAttr :public FTableRowBase {
	GENERATED_BODY()
public:
	FNavButtonAttr() {};

	//�ð�ť����ʽ  
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName ButtonType;

	//��ť��������ͼƬ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName NormalIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName HoverIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName PressedIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName HotKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ButtonSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName ClickedEvent;  //ί�к��¼��Ĺ�ϵ 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName OpenWidgetType;

};




/**
 *
 */
UCLASS()
class INVENTORY_API UCommonTypes : public UObject
{
	GENERATED_BODY()

};
