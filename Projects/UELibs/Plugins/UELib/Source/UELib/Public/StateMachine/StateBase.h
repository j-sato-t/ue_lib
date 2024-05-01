// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Manageable.h"
#include "StateBase.generated.h"

/**
 * 
 */
UCLASS()
class UELIB_API UStateBase : public UManageable
{
	GENERATED_BODY()
	
public:

	virtual bool IsTickable() const override;
};
