// Fill out your copyright notice in the Description page of Project Settings.


#include "Replay/ActionReplayComponent.h"

// Sets default values for this component's properties
UActionReplayComponent::UActionReplayComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	ActionReplay = nullptr;
	ReplayMode = EReplayMode::Invalid;
	ReplayLogName = TEXT("ActionReplay_Component.log");
}

void UActionReplayComponent::OnBoolRecord(FString action, bool data)
{
	OnBoolRecordBody.Broadcast(action, data);
}

void UActionReplayComponent::OnInt32Record(FString action, int32 data)
{
	OnInt32RecordBody.Broadcast(action, data);
}

void UActionReplayComponent::OnFloatRecord(FString action, float data)
{
	OnFloatRecordBody.Broadcast(action, data);
}

void UActionReplayComponent::OnVectorRecord(FString action, FVector data)
{
	OnVectorRecordBody.Broadcast(action, data);
}


// Called when the game starts
void UActionReplayComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	ActionReplay = NewObject<UActionReplay>(this);

	ActionReplay->Setup(ReplayMode, ReplayLogName);

	ActionReplay->OnBoolRecordBody.AddDynamic(this, &UActionReplayComponent::OnBoolRecord);
	ActionReplay->OnInt32RecordBody.AddDynamic(this, &UActionReplayComponent::OnInt32Record);
	ActionReplay->OnFloatRecordBody.AddDynamic(this, &UActionReplayComponent::OnFloatRecord);
	ActionReplay->OnVectorRecordBody.AddDynamic(this, &UActionReplayComponent::OnVectorRecord);

	ActionReplay->Open(FOpenSetting{
		ELogLevel::Info,
		TEXT(""),
	});
}

void UActionReplayComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ActionReplay != nullptr) {
		ActionReplay->Close();
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
	if (ActionReplay == nullptr) return;
	ActionReplay->SaveBool(action, data, force);
}

void UActionReplayComponent::SaveInt32(const FString& action, int32 data, bool force)
{
	if (ActionReplay == nullptr) return;
	ActionReplay->SaveInt32(action, data, force);
}

void UActionReplayComponent::SaveFloat(const FString& action, float data, bool force)
{
	if (ActionReplay == nullptr) return;
	ActionReplay->SaveFloat(action, data, force);
}

void UActionReplayComponent::SaveVector(const FString& action, const FVector& data, bool force)
{
	if (ActionReplay == nullptr) return;
	ActionReplay->SaveVector(action, data, force);
}
