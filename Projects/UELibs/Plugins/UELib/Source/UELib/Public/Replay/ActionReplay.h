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
struct SRecordStruct : public FJsonSerializable {
	FString Action;
	int32 DataType;
	T Data;
	int64 Time;

	SRecordStruct() : Action(TEXT("")), DataType(0), Data(), Time(0L) {}
	SRecordStruct(FString a, int32 dt, T d, int64 t) : Action(a), DataType(dt), Data(d), Time(t) {}

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("action", Action);
		JSON_SERIALIZE("dataType", DataType);
		JSON_SERIALIZE("data", Data);
		JSON_SERIALIZE("time", Time);
	END_JSON_SERIALIZER
};

struct SRecordJson {
	int64 Time;
	EActionDataType DataType;
	FString Json;

	SRecordJson(int64 t, EActionDataType dt, FString j) : Time(t), DataType(dt), Json(j) {}
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
	UActionLogWriter* _logWriter;

	/// <summary>
	/// ログを読み込むオブジェクト
	/// </summary>
	UPROPERTY()
	UActionLogReader* _logReader;

	/// <summary>
	/// 読み書きどちらで動作するか
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Replay, Meta = (ExposeOnSpawn = true))
	EReplayMode _replayMode;

	/// <summary>
	/// 読み書きするログファイルの名前
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Replay, Meta = (ExposeOnSpawn = true))
	FString _replayLogName;

	/// <summary>
	/// 経過時間
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Replay)
	int64 _playTime;

	// ----------------------- temp last value

	UPROPERTY()
	TMap<FString, bool> _lastBoolValues;

	UPROPERTY()
	TMap<FString, int32> _lastInt32Values;

	UPROPERTY()
	TMap<FString, float> _lastFloatValues;

	UPROPERTY()
	TMap<FString, FString> _lastVectorValues; // 保持は文字列で

	// ----------------------- read setting

	/// <summary>
	/// ファイルを開いた際に読み込む行数
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Replay|Load", Meta = (ExposeOnSpawn = true))
	int32 _initLoadLines;

	/// <summary>
	/// 次フレームから読み込む行数
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Replay|Load", Meta = (ExposeOnSpawn = true))
	int32 _frameLoadLines;

	bool _canReadLine;

	TArray<SRecordJson> _loadedLines;
	int32 _lineIndex;


	// ----------------------- on read line callback
public:
	/// <summary>
	/// Bool型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnBoolRecord _onBoolRecord;

	/// <summary>
	/// int32型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnInt32Record _onInt32Record;

	/// <summary>
	/// float型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnFloatRecord _onFloatRecord;

	/// <summary>
	/// FVector型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnVectorRecord _onVectorRecord;

	// ----------------------- save functions
public:

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveBool(const FString& action, bool data, bool force);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveInt32(const FString& action, int32 data, bool force);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveFloat(const FString& action, float data, bool force);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveVector(const FString& action, const FVector& data, bool force);

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
	void SaveRecord(const FString& action, EActionDataType dataType, T data, bool force, TMap<FString, T>* lastValues);
	

	// ----------------------- load functions
protected:
	void LoadLines(const int32 maxLineCount);

	void CheckRecords();
	void BroadcastRecord(const SRecordJson* record);

	template<typename T>
	void GetData(FString line, TFunction<void(const FString&, T)> callback);

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
	void Setup(EReplayMode mode, const FString& replayLogName = TEXT("ActionReplay.log"), int32 initLoadLine = 128, int32 frameLoadLine = 64);

};
