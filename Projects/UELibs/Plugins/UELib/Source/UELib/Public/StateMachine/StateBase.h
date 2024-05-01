// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Manageable.h"
#include "StateBase.generated.h"

class UStateBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransitionState, UStateBase*, State);

/**
 * 
 */
UCLASS(Blueprintable)
class UELIB_API UStateBase : public UManageable
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	FOnTransitionState OnNextState;

	UPROPERTY()
	FOnTransitionState OnPushState;


public:
	virtual bool IsTickable() const override;

protected:

	UFUNCTION(BlueprintCallable, Category = "UELib|StateMachine")
	void NextState(UStateBase* State);
	UFUNCTION(BlueprintCallable, Category = "UELib|StateMachine")
	void PushState(UStateBase* State);
};
