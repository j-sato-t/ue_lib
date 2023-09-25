// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/WaitableBase.h"
#include "Tasks/Task.h"
#include "WaitFunction.generated.h"

DECLARE_DELEGATE_RetVal(bool, FWaitFunction);

/**
 * 
 */
UCLASS()
class UELIB_API UWaitFunction : public UWaitableBase
{
	GENERATED_BODY()
private:
	FWaitFunction _waitFunction;
	UE::Tasks::TTask<void> _taskBody;

protected:
	virtual void StartTask_Implementation() override;

public:

	void SetFunction(TFunction<bool()> taskFunction);
};
