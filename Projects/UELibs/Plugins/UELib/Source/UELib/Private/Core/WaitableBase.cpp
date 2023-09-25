// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/WaitableBase.h"

void UWaitableBase::SetFinish()
{
	_status = ETaskStatus::Finished;
}

void UWaitableBase::SetFailed()
{
	_status = ETaskStatus::Failed;
}

void UWaitableBase::StartTask_Implementation()
{
}
