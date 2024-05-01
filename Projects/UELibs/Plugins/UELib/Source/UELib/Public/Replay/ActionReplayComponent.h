// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Replay/ActionReplay.h"
#include "ActionReplayComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UELIB_API UActionReplayComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UActionReplay* ActionReplay;

protected:
	/// <summary>
	/// Bool型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnBoolRecord OnBoolRecordBody;

	/// <summary>
	/// int32型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnInt32Record OnInt32RecordBody;

	/// <summary>
	/// float型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnFloatRecord OnFloatRecordBody;

	/// <summary>
	/// FVector型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnVectorRecord OnVectorRecordBody;

	/// <summary>
	/// 読み書きどちらで動作するか
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Replay)
	EReplayMode ReplayMode;

	/// <summary>
	/// 読み書きするログファイルの名前
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Replay)
	FString ReplayLogName;

public:	
	// Sets default values for this component's properties
	UActionReplayComponent();

private:
	UFUNCTION()
	void OnBoolRecord(FString Action, bool bData);
	UFUNCTION()
	void OnInt32Record(FString Action, int32 Data);
	UFUNCTION()
	void OnFloatRecord(FString Action, float Data);
	UFUNCTION()
	void OnVectorRecord(FString Action, FVector Data);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveBool(const FString& Action, bool bData, bool bForce);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveInt32(const FString& Action, int32 Data, bool bForce);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveFloat(const FString& Action, float Data, bool bForce);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveVector(const FString& Action, const FVector& Data, bool bForce);
};
