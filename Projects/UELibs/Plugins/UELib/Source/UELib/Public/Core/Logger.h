// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Logger.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUELib, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRelayLogString, FString, logString);


UENUM(BlueprintType)
enum class ELogLevel : uint8
{
	Trace = 0,
	Debug,
	Info,
	Warning,
	Error,
	Critical,
};

/**
*
*/
UCLASS(Blueprintable, BlueprintType)
class UELIB_API ULogger : public UObject
{
	GENERATED_BODY()
protected:
	/// <summary>
	/// これ未満のレベルのログを無視するフィルタ
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = Log)
	ELogLevel _filterType;

	/// <summary>
	/// ログ文字列に追加する抽出用タグ
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = Log)
	FString _tag;

	/// <summary>
	/// UTC時刻を使うか
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = Log)
	bool _timeUTC;

public:
	/// <summary>
	/// 生成したログ文字列を中継するデリゲート
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = Log)
	FRelayLogString OnRelayLogString;

public:
	ULogger();

private:

	/// <summary>
	/// ログ文字列を生成する
	/// </summary>
	/// <param name="level">ログレベル</param>
	/// <param name="msg">メッセージの内容</param>
	/// <returns></returns>
	FString BuildLogMsg(ELogLevel level, FString msg) const;

	/// <summary>
	/// ログレベルのenumを文字列にする
	/// </summary>
	/// <param name="level">対象のレベル</param>
	/// <returns>文字列化したレベル</returns>
	FString GetLevelString(ELogLevel level) const;

	/// <summary>
	/// 現在時刻を文字列として取得
	/// </summary>
	/// <returns>ISO8601形式の文字列（タイムゾーンはZ固定）</returns>
	FString GetTimeString() const;

	/// <summary>
	/// 出力するかの判定と文字列の生成
	/// </summary>
	/// <param name="level">ログレベル</param>
	/// <param name="msg">ログ内容</param>
	/// <param name="outMsg">出力する文字列</param>
	/// <returns>出力するか</returns>
	bool CheckAndBuildMsg(ELogLevel level, FString msg, FString& outMsg) const;

public:

	// =================================

	UFUNCTION(BlueprintCallable, Category = Log)
	void LogTrace(FString msg) const;
	UFUNCTION(BlueprintCallable, Category = Log)
	void LogDebug(FString msg) const;
	UFUNCTION(BlueprintCallable, Category = Log)
	void LogInfo(FString msg) const;
	UFUNCTION(BlueprintCallable, Category = Log)
	void LogWarning(FString msg) const;
	UFUNCTION(BlueprintCallable, Category = Log)
	void LogError(FString msg) const;
	UFUNCTION(BlueprintCallable, Category = Log)
	void LogCritical(FString msg, bool bIsClash = false) const;

	// コードで初期化する時用
	void Setup(ELogLevel filter = ELogLevel::Info, FString tag = TEXT(""), bool timeUTC = true);
};
