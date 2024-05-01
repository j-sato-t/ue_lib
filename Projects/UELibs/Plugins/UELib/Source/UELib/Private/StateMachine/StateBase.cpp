// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/StateBase.h"

bool UStateBase::IsTickable() const
{
	// オブジェクト単体でTickはさせない→マシンから呼ぶ想定
	return false;
}

void UStateBase::NextState(UStateBase* State)
{
	if (!IsValid(State)) {
		Logger->LogError(TEXT("fail NextState"));
		return;
	}
	OnNextState.Broadcast(State);
}

void UStateBase::PushState(UStateBase* State)
{
	if (!IsValid(State)) {
		Logger->LogError(TEXT("fail PushState"));
		return;
	}
	OnPushState.Broadcast(State);
}
