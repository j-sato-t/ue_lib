// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionLogReader.generated.h"

#define MAX_LINE_LENGTH 512

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UActionLogReader : public UObject
{
	GENERATED_BODY()
private:
	FILE* FilePointer;
	char LineBuff[MAX_LINE_LENGTH];

public:
	/**
		open log file  
		@note keep lock
	*/
	UFUNCTION(BlueprintCallable, Category = "Replay|Load")
	bool OpenFile(FString FileName);
	// read one line
	UFUNCTION(BlueprintCallable, Category = "Replay|Load")
	bool ReadLine(FString& OutLine);

	virtual void BeginDestroy() override;
};
