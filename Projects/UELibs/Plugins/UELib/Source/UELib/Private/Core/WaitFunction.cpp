// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/WaitFunction.h"

void UWaitFunction::StartTask_Implementation()
{
	if (!WaitFunction.IsBound()) {
		SetFailed();
		return;
	}

	TaskBody = UE::Tasks::Launch(UE_SOURCE_LOCATION, [this] {
		if (WaitFunction.Execute()) {
			SetFinish();
		}
		else {
			SetFailed();
		}
	});	
}

void UWaitFunction::SetFunction(TFunction<bool()> taskFunction)
{
	WaitFunction.BindLambda(taskFunction);
}
