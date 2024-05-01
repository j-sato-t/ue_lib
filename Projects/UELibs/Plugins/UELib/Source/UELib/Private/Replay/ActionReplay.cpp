// Fill out your copyright notice in the Description page of Project Settings.


#include "Replay/ActionReplay.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetStringLibrary.h>

UActionReplay::UActionReplay() : Super()
{
	ReplayMode = EReplayMode::Invalid;
	ReplayLogName = TEXT("ActionLog.log");
	PlayTime = 0L;

	InitLoadLines = 128;
	FrameLoadLines = 64;
}

void UActionReplay::SaveBool(const FString& action, bool data, bool force)
{
	SaveRecord<bool>(action, EActionDataType::Bool, data, force, &LastBoolValues);
}

void UActionReplay::SaveInt32(const FString& action, int32 data, bool force)
{
	SaveRecord<int32>(action, EActionDataType::Int32, data, force, &LastInt32Values);
}

void UActionReplay::SaveFloat(const FString& action, float data, bool force)
{
	SaveRecord<float>(action, EActionDataType::Float, data, force, &LastFloatValues);
}

void UActionReplay::SaveVector(const FString& action, const FVector& data, bool force)
{
	SaveRecord<FString>(action, EActionDataType::Vector, data.ToString(), force, &LastVectorValues);
}

template<typename T>
void UActionReplay::SaveRecord(const FString& action, EActionDataType dataType, T data, bool force, TMap<FString, T>* lastValues)
{
	if (IsShipping()) return;
	if (ReplayMode != EReplayMode::Record) return;
	if (!IsRunning()) return;

	if (!force) {
		T* lastValue = lastValues->Find(action);
		if (lastValue != nullptr && *lastValue == data) return;
	}
	lastValues->Add(action, data);

	FRecordStruct<T> record(action, (int32)dataType, data, PlayTime);
	FString json = record.ToJson(false);
	LogWriter->AddLogString(json);
	Logger->LogDebug(FString::Printf(TEXT("add to action log: %s"), *json));
}

void UActionReplay::LoadLines(const int32 maxLineCount)
{
	if (IsShipping()) return;
	if (!IsRunning()) return;
	if (ReplayMode != EReplayMode::Play || !bCanReadLine) return;

	FString readLine;

	for (int32 i = 0; i < maxLineCount; i++) {
		if (LogReader->ReadLine(readLine)) {
			Logger->LogDebug(FString::Printf(TEXT("read line: %s"), *readLine));

			TSharedPtr<FJsonObject> jsonRoot = MakeShareable(new FJsonObject());
			TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(readLine);

			if (FJsonSerializer::Deserialize(jsonReader, jsonRoot)) {
				LoadedLines.Add(FRecordJson(jsonRoot->GetIntegerField(TEXT("time")), (EActionDataType)jsonRoot->GetIntegerField(TEXT("dataType")), readLine));
			}
		}
		else {
			bCanReadLine = false;
			Logger->LogInfo(TEXT("stop read"));
			break;
		}
	}
}

void UActionReplay::CheckRecords()
{
	if (IsShipping()) return;
	if (ReplayMode != EReplayMode::Play) return;
	if (!IsRunning()) return;

	if (LoadedLines.Num() <= LineIndex) return;

	FRecordJson nowRecord = LoadedLines[LineIndex];
	if (nowRecord.Time > PlayTime) return;

	BroadcastRecord(&nowRecord);
	LineIndex++;
}

void UActionReplay::BroadcastRecord(const FRecordJson* record)
{
	switch (record->DataType) {
	case EActionDataType::Bool:
		GetData<bool>(record->Json, [this](FString action, bool data) {OnBoolRecordBody.Broadcast(action, data); });
		break;
	case EActionDataType::Int32:
		GetData<int32>(record->Json, [this](FString action, int32 data) {OnInt32RecordBody.Broadcast(action, data); });
		break;
	case EActionDataType::Float:
		GetData<float>(record->Json, [this](FString action, float data) {OnFloatRecordBody.Broadcast(action, data); });
		break;
	case EActionDataType::Vector:
		GetData<FString>(record->Json, [this](FString action, FString data) {
			FVector outVec;
			bool outvalid;
			UKismetStringLibrary::Conv_StringToVector(data, outVec, outvalid);
			if (outvalid) {
				OnVectorRecordBody.Broadcast(action, outVec);
			}
			else {
				Logger->LogWarning(FString::Printf(TEXT("fail conv vector: %s"), *data));
			}
		});
		break;
	}
}

template<typename T>
void UActionReplay::GetData(FString line, TFunction<void(const FString&, T)> callback)
{
	FRecordStruct<T> record;
	record.FromJson(line);

	callback(record.Action, record.Data);
}

bool UActionReplay::OnOpen_Implementation()
{
	if (IsShipping()) return false;

	switch (ReplayMode) {
	case EReplayMode::Record:
		LogWriter = NewObject<UActionLogWriter>(this);
		LogWriter->Setting(ReplayLogName);
		break;
	case EReplayMode::Play:
		LogReader = NewObject<UActionLogReader>(this);
		if (LogReader->OpenFile(ReplayLogName)) {
			LoadLines(InitLoadLines);
		}
		else {
			bCanReadLine = false;
			Logger->LogError(FString::Printf(TEXT("fail open action log file: %s"), *ReplayLogName));
			return false;
		}
		break;
		
	default:
		Logger->LogWarning(TEXT("ReplayMode is invalid"));
		return false;
	}

	return true;
}

void UActionReplay::OnReady_Implementation()
{
	SetTickable(true);
}

bool UActionReplay::OnPause_Implementation()
{
	SetTickable(false);
	return true;
}

bool UActionReplay::OnResume_Implementation()
{
	SetTickable(bCanReadLine);
	return true;
}

void UActionReplay::OnClose_Implementation()
{
	// ファイルのクローズなどは各オブジェクトで行われるためここでは明示的には呼ばない
}

void UActionReplay::OnTick_Implementation(float deltaTime)
{
	if (LogReader != nullptr) {
		LoadLines(FrameLoadLines);
	}

	PlayTime += deltaTime * 1000;
}

bool UActionReplay::IsShipping() const
{
	return UKismetSystemLibrary::GetBuildConfiguration().Equals(TEXT("Shipping"), ESearchCase::IgnoreCase);
}

void UActionReplay::Setup(EReplayMode mode, const FString& replayLogName, int32 initLoadLine, int32 frameLoadLine)
{
	if (!IsCreated()) {
		Logger->LogError(TEXT("call Setup, but after open"));
		SetFailed();
		return;
	}

	ReplayMode = mode;
	ReplayLogName = replayLogName;
	InitLoadLines = initLoadLine;
	FrameLoadLines = frameLoadLine;
}
