// Fill out your copyright notice in the Description page of Project Settings.


#include "Replay/ActionReplay.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetStringLibrary.h>

UActionReplay::UActionReplay() : Super()
{
	_replayMode = EReplayMode::Invalid;
	_replayLogName = TEXT("ActionLog.log");
	_playTime = 0L;

	_initLoadLines = 128;
	_frameLoadLines = 64;
}

void UActionReplay::SaveBool(const FString& action, bool data, bool force)
{
	SaveRecord<bool>(action, EActionDataType::Bool, data, force, &_lastBoolValues);
}

void UActionReplay::SaveInt32(const FString& action, int32 data, bool force)
{
	SaveRecord<int32>(action, EActionDataType::Int32, data, force, &_lastInt32Values);
}

void UActionReplay::SaveFloat(const FString& action, float data, bool force)
{
	SaveRecord<float>(action, EActionDataType::Float, data, force, &_lastFloatValues);
}

void UActionReplay::SaveVector(const FString& action, const FVector& data, bool force)
{
	SaveRecord<FString>(action, EActionDataType::Vector, data.ToString(), force, &_lastVectorValues);
}

template<typename T>
void UActionReplay::SaveRecord(const FString& action, EActionDataType dataType, T data, bool force, TMap<FString, T>* lastValues)
{
	if (IsShipping()) return;
	if (_replayMode != EReplayMode::Record) return;
	if (!IsRunning()) return;

	if (!force) {
		T* lastValue = lastValues->Find(action);
		if (lastValue != nullptr && *lastValue == data) return;
	}
	lastValues->Add(action, data);

	SRecordStruct<T> record(action, (int32)dataType, data, _playTime);
	FString json = record.ToJson(false);
	_logWriter->AddLogString(json);
	_logger->LogDebug(FString::Printf(TEXT("add to action log: %s"), *json));
}

void UActionReplay::LoadLines(const int32 maxLineCount)
{
	if (IsShipping()) return;
	if (!IsRunning()) return;
	if (_replayMode != EReplayMode::Play || !_canReadLine) return;

	FString readLine;

	for (int32 i = 0; i < maxLineCount; i++) {
		if (_logReader->ReadLine(readLine)) {
			_logger->LogDebug(FString::Printf(TEXT("read line: %s"), *readLine));

			TSharedPtr<FJsonObject> jsonRoot = MakeShareable(new FJsonObject());
			TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(readLine);

			if (FJsonSerializer::Deserialize(jsonReader, jsonRoot)) {
				_loadedLines.Add(SRecordJson(jsonRoot->GetIntegerField(TEXT("time")), (EActionDataType)jsonRoot->GetIntegerField(TEXT("dataType")), readLine));
			}
		}
		else {
			_canReadLine = false;
			_logger->LogInfo(TEXT("stop read"));
			break;
		}
	}
}

void UActionReplay::CheckRecords()
{
	if (IsShipping()) return;
	if (_replayMode != EReplayMode::Play) return;
	if (!IsRunning()) return;

	if (_loadedLines.Num() <= _lineIndex) return;

	SRecordJson nowRecord = _loadedLines[_lineIndex];
	if (nowRecord.Time > _playTime) return;

	BroadcastRecord(&nowRecord);
	_lineIndex++;
}

void UActionReplay::BroadcastRecord(const SRecordJson* record)
{
	switch (record->DataType) {
	case EActionDataType::Bool:
		GetData<bool>(record->Json, [this](FString action, bool data) {_onBoolRecord.Broadcast(action, data); });
		break;
	case EActionDataType::Int32:
		GetData<int32>(record->Json, [this](FString action, int32 data) {_onInt32Record.Broadcast(action, data); });
		break;
	case EActionDataType::Float:
		GetData<float>(record->Json, [this](FString action, float data) {_onFloatRecord.Broadcast(action, data); });
		break;
	case EActionDataType::Vector:
		GetData<FString>(record->Json, [this](FString action, FString data) {
			FVector outVec;
			bool outvalid;
			UKismetStringLibrary::Conv_StringToVector(data, outVec, outvalid);
			if (outvalid) {
				_onVectorRecord.Broadcast(action, outVec);
			}
			else {
				_logger->LogWarning(FString::Printf(TEXT("fail conv vector: %s"), *data));
			}
		});
		break;
	}
}

template<typename T>
void UActionReplay::GetData(FString line, TFunction<void(const FString&, T)> callback)
{
	SRecordStruct<T> record;
	record.FromJson(line);

	callback(record.Action, record.Data);
}

bool UActionReplay::OnOpen_Implementation()
{
	if (IsShipping()) return false;

	switch (_replayMode) {
	case EReplayMode::Record:
		_logWriter = NewObject<UActionLogWriter>(this);
		_logWriter->Setting(_replayLogName);
		break;
	case EReplayMode::Play:
		_logReader = NewObject<UActionLogReader>(this);
		if (_logReader->OpenFile(_replayLogName)) {
			LoadLines(_initLoadLines);
		}
		else {
			_canReadLine = false;
			_logger->LogError(FString::Printf(TEXT("fail open action log file: %s"), *_replayLogName));
			return false;
		}
		break;
		
	default:
		_logger->LogWarning(TEXT("ReplayMode is invalid"));
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
	SetTickable(_canReadLine);
	return true;
}

void UActionReplay::OnClose_Implementation()
{
	// ファイルのクローズなどは各オブジェクトで行われるためここでは明示的には呼ばない
}

void UActionReplay::OnTick_Implementation(float deltaTime)
{
	if (_logReader != nullptr) {
		LoadLines(_frameLoadLines);
	}

	_playTime += deltaTime * 1000;
}

bool UActionReplay::IsShipping() const
{
	return UKismetSystemLibrary::GetBuildConfiguration().Equals(TEXT("Shipping"), ESearchCase::IgnoreCase);
}

void UActionReplay::Setup(EReplayMode mode, const FString& replayLogName, int32 initLoadLine, int32 frameLoadLine)
{
	if (!IsCreated()) {
		_logger->LogError(TEXT("call Setup, but after open"));
		SetFailed();
		return;
	}

	_replayMode = mode;
	_replayLogName = replayLogName;
	_initLoadLines = initLoadLine;
	_frameLoadLines = frameLoadLine;
}
