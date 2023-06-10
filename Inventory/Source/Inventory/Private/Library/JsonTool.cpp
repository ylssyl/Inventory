// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/JsonTool.h"
#include "Kismet/GameplayStatics.h"
#include "JsonObjectConverter.h"

//JsonStr -> FGameSaveData
FGameSaveData FJsonTool::GetGameSaveDataFormJsonStr(FString InJsonStr)
{
	FGameSaveData GameSaveData;
	//共享指针(智能指针)
	TSharedPtr<FJsonValue> JsonValue;  //Root Value
	auto JsonReader = TJsonReaderFactory<>::Create(InJsonStr);
	if (FJsonSerializer::Deserialize(JsonReader, JsonValue))
	{
		TSharedPtr<FJsonObject> GameSaveDataJsonObj = JsonValue->AsObject();
		TSharedPtr<FJsonObject> PlayerDataJsonObj = GameSaveDataJsonObj->GetObjectField(FString("PlayerData"));
		GameSaveData.PlayerData.Name = FName(*PlayerDataJsonObj->GetStringField(FString("Name")));  //"zhangsan"
		GameSaveData.PlayerData.Level = PlayerDataJsonObj->GetIntegerField(FString("Level"));//1
		TArray<TSharedPtr<FJsonValue>> LocationJsonArray = PlayerDataJsonObj->GetArrayField(FString("Location"));
		GameSaveData.PlayerData.Location.X = LocationJsonArray[0]->AsNumber();
		GameSaveData.PlayerData.Location.Y = LocationJsonArray[1]->AsNumber();
		GameSaveData.PlayerData.Location.Z = LocationJsonArray[2]->AsNumber();
		TSharedPtr<FJsonObject> BagDataJsonObj = PlayerDataJsonObj->GetObjectField(FString("BagData"));
		GameSaveData.PlayerData.BagData.GridNums = BagDataJsonObj->GetIntegerField(FString("GridNum"));
		TArray<TSharedPtr<FJsonValue>> HadItemValueArray = BagDataJsonObj->GetArrayField(FString("HadItems"));
		for (TSharedPtr<FJsonValue> HadItemValue : HadItemValueArray)
		{
			GameSaveData.PlayerData.BagData.HadItems.Add((int)(HadItemValue->AsNumber()));
		}
		TArray<TSharedPtr<FJsonValue>> GridDatasValueArray = BagDataJsonObj->GetArrayField(FString("GridDatas"));
		for (TSharedPtr<FJsonValue> GridDatasValue : GridDatasValueArray)
		{
			GameSaveData.PlayerData.BagData.GridDatas.Add(
				FBagGridData(
					GridDatasValue->AsObject()->GetIntegerField(FString("ID")),
					GridDatasValue->AsObject()->GetIntegerField(FString("Num")))
			);
		}
		TArray<TSharedPtr<FJsonValue>> GroundDataValueArray = GameSaveDataJsonObj->GetArrayField(FString("GroundData"));

		for (TSharedPtr<FJsonValue> GroundDataValue : GroundDataValueArray)
		{
			TSharedPtr<FJsonObject> GroundDataJsonObj = GroundDataValue->AsObject();
			FName Index = FName(*GroundDataJsonObj->GetStringField(FString("Index")));
			int ID = GroundDataJsonObj->GetIntegerField(FString("ID"));
			int Num = GroundDataJsonObj->GetIntegerField(FString("Num"));
			TArray<TSharedPtr<FJsonValue>> LocationValueArray = GroundDataJsonObj->GetArrayField(FString("Location"));
			FVector Location = FVector(
				LocationValueArray[0]->AsNumber(),
				LocationValueArray[1]->AsNumber(),
				LocationValueArray[2]->AsNumber()
			);
			FItemOnGroundData ItemOnGroundData = FItemOnGroundData(Index, ID, Num, Location);
			GameSaveData.GroundData.ItemOnGroundDataMap.Add(Index, ItemOnGroundData);
		}

		TSharedPtr<FJsonObject> ShortcutsDataJsonObj = GameSaveDataJsonObj->GetObjectField(FString("ShortcutsData"));
		//GameSaveData.ShortcutsData.ShortcutsGridNums = ShortcutsDataJsonObj->GetIntegerField(FString("ShortcutsGridNums"));
		TArray<TSharedPtr<FJsonValue>> ShortcutsGridDatasValueArray = ShortcutsDataJsonObj->GetArrayField(FString("ShortcutsGridDatas"));
		for (TSharedPtr<FJsonValue> ShortcutsGridDatasValue : ShortcutsGridDatasValueArray)
		{
			GameSaveData.ShortcutsData.ShortcutsGridDatas.Add(
				FShortcutsGridData(
					ShortcutsGridDatasValue->AsObject()->GetIntegerField(FString("ShortcutsGridDataID")),
					ShortcutsGridDatasValue->AsObject()->GetIntegerField(FString("ShortcutsGridDataNum")))
			);
		}

		TArray<TSharedPtr<FJsonValue>> EquipmentDataValueArray = GameSaveDataJsonObj->GetArrayField(FString("EquipmentData"));
		for (TSharedPtr<FJsonValue> EquipmentDataValue : EquipmentDataValueArray)
		{
			TSharedPtr<FJsonObject> EquipmentDataJsonObj = EquipmentDataValue->AsObject();
			FName EquipmentGridPart = FName(*EquipmentDataJsonObj->GetStringField(FString("EquipmentGridPart")));
			int EquipmentGridDataID = EquipmentDataJsonObj->GetIntegerField(FString("EquipmentGridDataID"));
			int EquipmentGridDataNum = EquipmentDataJsonObj->GetIntegerField(FString("EquipmentGridDataNum"));
			FEquipmentGridData EquipmentGridData = FEquipmentGridData(EquipmentGridDataID, EquipmentGridDataNum);
			GameSaveData.EquipmentData.EquipmentDataMap.Add(EquipmentGridPart, EquipmentGridData);
		}
	}

	////InJsonStr->FGameSaveData
	//FGameSaveData GameSaveData;
	//FJsonObjectConverter::JsonObjectStringToUStruct<FGameSaveData>(InJsonStr, &GameSaveData, 0, 0);

	return GameSaveData;
}

//UStruct -> JsonStr
FString FJsonTool::GetJsonStrFormGameSaveData(FGameSaveData GameSaveData)
{
	FString OutJsonStr;
	auto JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJsonStr);

	JsonWriter->WriteObjectStart();//{
	{
		JsonWriter->WriteIdentifierPrefix("PlayerData");  //"PlayerData"
		JsonWriter->WriteObjectStart();//{
		{
			JsonWriter->WriteValue<FString>(FString("Name"), GameSaveData.PlayerData.Name.ToString());

			JsonWriter->WriteValue<int>(FString("Level"), GameSaveData.PlayerData.Level);

			JsonWriter->WriteIdentifierPrefix("Location");//"Location"
			JsonWriter->WriteArrayStart();//[
			JsonWriter->WriteValue<float>(GameSaveData.PlayerData.Location.X);
			JsonWriter->WriteValue<float>(GameSaveData.PlayerData.Location.Y);
			JsonWriter->WriteValue<float>(GameSaveData.PlayerData.Location.Z);
			JsonWriter->WriteArrayEnd();//]
			JsonWriter->WriteIdentifierPrefix("BagData"); //"BagData"
			JsonWriter->WriteObjectStart();//{
			{
				JsonWriter->WriteValue<int>(FString("GridNum"), GameSaveData.PlayerData.BagData.GridNums);

				JsonWriter->WriteIdentifierPrefix("HadItems");//"HadItems"
				JsonWriter->WriteArrayStart();//[
				for (int HandItem : GameSaveData.PlayerData.BagData.HadItems)
				{
					JsonWriter->WriteValue<int>(HandItem);
				}
				JsonWriter->WriteArrayEnd();//]
				JsonWriter->WriteIdentifierPrefix("GridDatas");//"GridDatas"
				JsonWriter->WriteArrayStart();//[
				for (FBagGridData BagGridData : GameSaveData.PlayerData.BagData.GridDatas)
				{
					JsonWriter->WriteObjectStart();//{
					{
						JsonWriter->WriteValue<int>(FString("ID"), BagGridData.ID);
						JsonWriter->WriteValue<int>(FString("Num"), BagGridData.Num);
					}
					JsonWriter->WriteObjectEnd();//}
				}
				JsonWriter->WriteArrayEnd();//]
			}
			JsonWriter->WriteObjectEnd();//}
		}
		JsonWriter->WriteObjectEnd();//}
		//地上的数据
		JsonWriter->WriteIdentifierPrefix("GroundData");//"GroundData"
		JsonWriter->WriteArrayStart();//[
		for (TPair<FName, FItemOnGroundData> KeyValuePair : GameSaveData.GroundData.ItemOnGroundDataMap)
		{
			JsonWriter->WriteObjectStart();//{
			{
				JsonWriter->WriteValue<FString>(FString("Index"), KeyValuePair.Key.ToString());
				JsonWriter->WriteValue<int>(FString("ID"), KeyValuePair.Value.ID);
				JsonWriter->WriteValue<int>(FString("Num"), KeyValuePair.Value.Num);
				JsonWriter->WriteIdentifierPrefix("Location");//"Location"
				JsonWriter->WriteArrayStart();//[
				JsonWriter->WriteValue<float>(KeyValuePair.Value.Location.X);
				JsonWriter->WriteValue<float>(KeyValuePair.Value.Location.Y);
				JsonWriter->WriteValue<float>(KeyValuePair.Value.Location.Z);
				JsonWriter->WriteArrayEnd();//]
			}
			JsonWriter->WriteObjectEnd();//}
		}
		JsonWriter->WriteArrayEnd();//]
		JsonWriter->WriteIdentifierPrefix("ShortcutsData");//"ShortcutsData"
		JsonWriter->WriteObjectStart();//{
		JsonWriter->WriteIdentifierPrefix("ShortcutsGridDatas");//"ShortcutsGridDatas"
		JsonWriter->WriteArrayStart();//[
		for (FShortcutsGridData ShortcutsGridData : GameSaveData.ShortcutsData.ShortcutsGridDatas)
		{
			JsonWriter->WriteObjectStart();//{
			{
				JsonWriter->WriteValue<int>(FString("ShortcutsGridDataID"), ShortcutsGridData.ShortcutsGridDataID);
				JsonWriter->WriteValue<int>(FString("ShortcutsGridDataNum"), ShortcutsGridData.ShortcutsGridDataNum);
			}
			JsonWriter->WriteObjectEnd();//}
		}
		JsonWriter->WriteArrayEnd();//]
		JsonWriter->WriteObjectEnd();//}
		JsonWriter->WriteIdentifierPrefix("EquipmentData");//"EquipmentData"
		JsonWriter->WriteArrayStart();//[
		for (TPair< FName, FEquipmentGridData > KeyValuePair : GameSaveData.EquipmentData.EquipmentDataMap)
		{
			JsonWriter->WriteObjectStart();//{
			{
				JsonWriter->WriteValue<FString>(FString("EquipmentGridPart"), KeyValuePair.Key.ToString());
				JsonWriter->WriteValue<int>(FString("EquipmentGridDataID"), KeyValuePair.Value.EquipmentGridDataID);
				JsonWriter->WriteValue<int>(FString("EquipmentGridDataNum"), KeyValuePair.Value.EquipmentGridDataNum);
			}
			JsonWriter->WriteObjectEnd();//}
		}
		JsonWriter->WriteArrayEnd();//]

	}
	JsonWriter->WriteObjectEnd();//}
	JsonWriter->Close();//关闭写入流

	//UKismetSystemLibrary::PrintString(0, OutJsonStr);
	//FString OutJsonStr;
	//FJsonObjectConverter::UStructToJsonObjectString<FGameSaveData>(GameSaveData, OutJsonStr);
	return OutJsonStr;
}
