// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionLogWriter.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UActionLogWriter : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	FString FilePath;

	UPROPERTY()
	TArray<FString> Buffer;

	UPROPERTY(BlueprintReadOnly, Category = "UELib|ActionReplay|Save")
	int32 BufferMaxSize;

public:
	UActionLogWriter();

	// Setup save file path
	UFUNCTION(BlueprintCallable, Category = "UELib|ActionReplay|Save")
	void Setting(FString FileName, int32 BufferSize = 32);

	// Append string to log buffer
	UFUNCTION(BlueprintCallable, Category = "UELib|ActionReplay|Save")
	void AddLogString(FString LogString);

	/**
		Write file from buffer  
		@note auto call from BeginDestroy
	*/
	UFUNCTION(BlueprintCallable, Category = "UELib|ActionReplay|Save")
	bool WriteToFile();

	virtual void BeginDestroy() override;
};
