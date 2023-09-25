// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/TickableObject.h"
#include "WaitableBase.generated.h"

UENUM()
enum class ETaskStatus : uint8 {
	Running = 0,
	Finished,
	Failed,
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UELIB_API UWaitableBase : public UTickableObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
	ETaskStatus _status = ETaskStatus::Running;
protected:
	UFUNCTION(BlueprintCallable, Category=Task)
	void SetFinish();
	UFUNCTION(BlueprintCallable, Category = Task)
	void SetFailed();
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Task)
	void StartTask();
	virtual void StartTask_Implementation();

	UFUNCTION(BlueprintPure, Category = Task)
	bool IsSuccessed() { return _status == ETaskStatus::Finished; }
	UFUNCTION(BlueprintPure, Category = Task)
	bool IsFailed() { return _status == ETaskStatus::Failed; }
};
