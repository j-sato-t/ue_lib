// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Manageable.h"
#include "ManageableTest.generated.h"

/**
 * 
 */
UCLASS()
class UELIBS_API UManageableTest : public UManageable
{
	GENERATED_BODY()
protected:
	virtual bool OnOpen_Implementation() override;
};
