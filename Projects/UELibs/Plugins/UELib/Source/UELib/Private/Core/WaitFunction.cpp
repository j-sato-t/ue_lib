// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/WaitFunction.h"

void UWaitFunction::StartTask_Implementation()
{
	if (!_waitFunction.IsBound()) {
		SetFailed();
		return;
	}

	_taskBody = UE::Tasks::Launch(UE_SOURCE_LOCATION, [this] {
		if (_waitFunction.Execute()) {
			SetFinish();
		}
		else {
			SetFailed();
		}
	});	
}

void UWaitFunction::SetFunction(TFunction<bool()> taskFunction)
{
	_waitFunction.BindLambda(taskFunction);
}
