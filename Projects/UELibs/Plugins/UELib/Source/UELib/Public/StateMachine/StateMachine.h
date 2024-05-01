// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Manageable.h"
#include "StateMachine.generated.h"

class UStateBase;

/**
 * 
 */
UCLASS()
class UELIB_API UStateMachine : public UManageable
{
	GENERATED_BODY()
public:

private:
	UFUNCTION()
	void OnNextState(UStateBase* NextState);
	UFUNCTION()
	void OnPushState(UStateBase* PushState);

private:
	UPROPERTY()
	TArray<UStateBase*> StateStack;
};
