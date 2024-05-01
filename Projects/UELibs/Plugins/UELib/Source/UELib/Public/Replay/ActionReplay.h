// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Manageable.h"
#include "Replay/ActionLogReader.h"
#include "Replay/ActionLogWriter.h"
#include "ActionReplay.generated.h"

class UActionLogWriter;
class UActionLogReader;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoolRecord, FString, Action, bool, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInt32Record, FString, Action, int32, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFloatRecord, FString, Action, float, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVectorRecord, FString, Action, FVector, Data);

UENUM(BlueprintType)
enum class EReplayMode : uint8 {
	Invalid,
	Record,
	Play,
};

UENUM()
enum class EActionDataType : uint8 {
	Bool,
	Int32,
	Float,
	Vector,
};

// json structure
template <typename T>
struct FRecordStruct : public FJsonSerializable {
	FString Action;
	int32 DataType;
	T Data;
	int64 Time;

	FRecordStruct() : Action(TEXT("")), DataType(0), Data(), Time(0L) {}
	FRecordStruct(FString a, int32 dt, T d, int64 t) : Action(a), DataType(dt), Data(d), Time(t) {}

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("action", Action);
		JSON_SERIALIZE("dataType", DataType);
		JSON_SERIALIZE("data", Data);
		JSON_SERIALIZE("time", Time);
	END_JSON_SERIALIZER
};

struct FRecordJson {
	int64 Time;
	EActionDataType DataType;
	FString Json;

	FRecordJson(int64 t, EActionDataType dt, FString j) : Time(t), DataType(dt), Json(j) {}
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UELIB_API UActionReplay : public UManageable
{
	GENERATED_BODY()
public:
	UActionReplay();

protected:
	/// <summary>
	/// ログを書き込むオブジェクト
	/// </summary>
	UPROPERTY()
	UActionLogWriter* LogWriter;

	/// <summary>
	/// ログを読み込むオブジェクト
	/// </summary>
	UPROPERTY()
	UActionLogReader* LogReader;

	/// <summary>
	/// 読み書きどちらで動作するか
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UELib|ActionReplay", Meta = (ExposeOnSpawn = true))
	EReplayMode ReplayMode;

	/// <summary>
	/// 読み書きするログファイルの名前
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UELib|ActionReplay", Meta = (ExposeOnSpawn = true))
	FString ReplayLogName;

	/// <summary>
	/// 経過時間
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "UELib|ActionReplay")
	int64 PlayTime;

	// ----------------------- temp last value

	UPROPERTY()
	TMap<FString, bool> LastBoolValues;

	UPROPERTY()
	TMap<FString, int32> LastInt32Values;

	UPROPERTY()
	TMap<FString, float> LastFloatValues;

	UPROPERTY()
	TMap<FString, FString> LastVectorValues; // 保持は文字列で

	// ----------------------- read setting

	/// <summary>
	/// ファイルを開いた際に読み込む行数
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Replay|Load", Meta = (ExposeOnSpawn = true))
	int32 InitLoadLines;

	/// <summary>
	/// 次フレームから読み込む行数
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Replay|Load", Meta = (ExposeOnSpawn = true))
	int32 FrameLoadLines;

	bool bCanReadLine;

	TArray<FRecordJson> LoadedLines;
	int32 LineIndex;


	// ----------------------- on read line callback
public:
	/// <summary>
	/// Bool型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnBoolRecord OnBoolRecordBody;

	/// <summary>
	/// int32型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnInt32Record OnInt32RecordBody;

	/// <summary>
	/// float型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnFloatRecord OnFloatRecordBody;

	/// <summary>
	/// FVector型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnVectorRecord OnVectorRecordBody;

	// ----------------------- save functions
public:

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveBool(const FString& Action, bool bData, bool bForce);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveInt32(const FString& Action, int32 Data, bool bForce);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveFloat(const FString& Action, float Data, bool bForce);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveVector(const FString& Action, const FVector& Data, bool bForce);

protected:

	/// <summary>
	/// 書き込み処理の共通部分
	/// </summary>
	/// <typeparam name="T">書き込む型</typeparam>
	/// <param name="action">アクション名</param>
	/// <param name="dataType">データ型（Tに対応したもの）</param>
	/// <param name="data">データ内容</param>
	/// <param name="force">直近と同じデータでも記録するか</param>
	/// <param name="lastValues">直近データが格納されたマップ</param>
	template<typename T>
	void SaveRecord(const FString& Ation, EActionDataType DataType, T Data, bool bForce, TMap<FString, T>* LastValues);
	

	// ----------------------- load functions
protected:
	void LoadLines(const int32 MaxLineCount);

	void CheckRecords();
	void BroadcastRecord(const FRecordJson* Record);

	template<typename T>
	void GetData(FString Line, TFunction<void(const FString&, T)> Callback);

	// ------------------------ manageable

	virtual bool OnOpen_Implementation() override;

	virtual void OnReady_Implementation() override;

	virtual bool OnPause_Implementation() override;

	virtual bool OnResume_Implementation() override;

	virtual void OnClose_Implementation() override;

	virtual void OnTick_Implementation(float deltaTime) override;

	// ------------------------ params

	bool IsShipping() const;

public:

	/// <summary>
	/// コード用初期化関数
	/// </summary>
	/// <param name="mode"></param>
	/// <param name="replayLogName"></param>
	/// <param name="initLoadLine"></param>
	/// <param name="frameLoadLine"></param>
	void Setup(EReplayMode mode, const FString& ReplayLogName = TEXT("ActionReplay.log"), int32 InitLoadLine = 128, int32 FrameLoadLine = 64);

};
