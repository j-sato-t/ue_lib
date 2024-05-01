// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/WaitableBase.h"

void UWaitableBase::SetFinish()
{
	Status = ETaskStatus::Finished;
}

void UWaitableBase::SetFailed()
{
	Status = ETaskStatus::Failed;
}

void UWaitableBase::StartTask_Implementation()
{
}
