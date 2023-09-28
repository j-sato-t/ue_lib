// Fill out your copyright notice in the Description page of Project Settings.


#include "Replay/ActionReplayComponent.h"

// Sets default values for this component's properties
UActionReplayComponent::UActionReplayComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	_actionReplay = nullptr;
	_replayMode = EReplayMode::Invalid;
	_replayLogName = TEXT("ActionReplay_Component.log");
}

void UActionReplayComponent::OnBoolRecord(FString action, bool data)
{
	_onBoolRecord.Broadcast(action, data);
}

void UActionReplayComponent::OnInt32Record(FString action, int32 data)
{
	_onInt32Record.Broadcast(action, data);
}

void UActionReplayComponent::OnFloatRecord(FString action, float data)
{
	_onFloatRecord.Broadcast(action, data);
}

void UActionReplayComponent::OnVectorRecord(FString action, FVector data)
{
	_onVectorRecord.Broadcast(action, data);
}


// Called when the game starts
void UActionReplayComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	_actionReplay = NewObject<UActionReplay>(this);

	_actionReplay->Setup(_replayMode, _replayLogName);

	_actionReplay->_onBoolRecord.AddDynamic(this, &UActionReplayComponent::OnBoolRecord);
	_actionReplay->_onInt32Record.AddDynamic(this, &UActionReplayComponent::OnInt32Record);
	_actionReplay->_onFloatRecord.AddDynamic(this, &UActionReplayComponent::OnFloatRecord);
	_actionReplay->_onVectorRecord.AddDynamic(this, &UActionReplayComponent::OnVectorRecord);

	_actionReplay->Open(FOpenSetting{
		ELogLevel::Info,
		TEXT(""),
	});
}

void UActionReplayComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (_actionReplay != nullptr) {
		_actionReplay->Close();
	}

	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UActionReplayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActionReplayComponent::SaveBool(const FString& action, bool data, bool force)
{
	if (_actionReplay == nullptr) return;
	_actionReplay->SaveBool(action, data, force);
}

void UActionReplayComponent::SaveInt32(const FString& action, int32 data, bool force)
{
	if (_actionReplay == nullptr) return;
	_actionReplay->SaveInt32(action, data, force);
}

void UActionReplayComponent::SaveFloat(const FString& action, float data, bool force)
{
	if (_actionReplay == nullptr) return;
	_actionReplay->SaveFloat(action, data, force);
}

void UActionReplayComponent::SaveVector(const FString& action, const FVector& data, bool force)
{
	if (_actionReplay == nullptr) return;
	_actionReplay->SaveVector(action, data, force);
}
