// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActionLogFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UActionLogFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// make log file path, and create directory
	static FString MakeLogFilePath(FString FileName);
};
