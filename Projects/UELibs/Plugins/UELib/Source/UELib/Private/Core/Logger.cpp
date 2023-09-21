// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Logger.h"

DEFINE_LOG_CATEGORY(LogUELib)


ULogger::ULogger()
{
	_tag = TEXT("Default");
	_filterType = ELogLevel::Info;
	_timeUTC = true;
}



FString ULogger::BuildLogMsg(ELogLevel level, FString msg) const
{
	return FString::Printf(TEXT("%s [%s] {%s}: %s"), *GetTimeString(), *GetLevelString(level), *_tag, *msg);
}

FString ULogger::GetLevelString(ELogLevel level) const
{
	FString full = UEnum::GetValueAsString(level);
	FString l, r;
	full.Split(TEXT("::"), &l, &r);
	return r;
}

FString ULogger::GetTimeString() const
{
	FDateTime now = _timeUTC ? FDateTime::UtcNow() : FDateTime::Now();
	return now.ToIso8601();
}

bool ULogger::CheckAndBuildMsg(ELogLevel level, FString msg, FString& outMsg) const
{
	if (level < _filterType) return false;
	outMsg = BuildLogMsg(level, msg);
	OnRelayLogString.Broadcast(outMsg);
	return true;
}

void ULogger::LogTrace(FString msg) const
{
	FString lm;
	if (CheckAndBuildMsg(ELogLevel::Trace, msg, lm)) UE_LOG(LogUELib, Log, TEXT("%s"), *lm);
}

void ULogger::LogDebug(FString msg) const
{
	FString lm;
	if (CheckAndBuildMsg(ELogLevel::Debug, msg, lm)) UE_LOG(LogUELib, Log, TEXT("%s"), *lm);
}

void ULogger::LogInfo(FString msg) const
{
	FString lm;
	if (CheckAndBuildMsg(ELogLevel::Info, msg, lm)) UE_LOG(LogUELib, Log, TEXT("%s"), *lm);
}

void ULogger::LogWarning(FString msg) const
{
	FString lm;
	if (CheckAndBuildMsg(ELogLevel::Warning, msg, lm)) UE_LOG(LogUELib, Warning, TEXT("%s"), *lm);
}

void ULogger::LogError(FString msg) const
{
	FString lm;
	if (CheckAndBuildMsg(ELogLevel::Error, msg, lm)) UE_LOG(LogUELib, Error, TEXT("%s"), *lm);
}

void ULogger::LogCritical(FString msg, bool bIsClash) const
{
	FString lm;
	if (CheckAndBuildMsg(ELogLevel::Critical, msg, lm))
	{
		if (bIsClash)
		{
			UE_LOG(LogUELib, Fatal, TEXT("%s"), *lm);
		}
		else
		{
			UE_LOG(LogUELib, Error, TEXT("%s"), *lm);
		}
	}
}
