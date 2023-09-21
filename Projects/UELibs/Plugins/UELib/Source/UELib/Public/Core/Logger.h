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
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category=Log)
	ELogLevel _filterType;
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = Log)
	FString _tag;
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = Log)
	bool _timeUTC;

public:
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

	FString GetLevelString(ELogLevel level) const;

	FString GetTimeString() const;

	bool CheckAndBuildMsg(ELogLevel level, FString msg, FString& outMsg) const;

public:

	// =================================

	UFUNCTION(BlueprintCallable, Category=Log)
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
};
