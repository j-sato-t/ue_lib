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
	UActionReplay* _actionReplay;

protected:
	/// <summary>
	/// Bool型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnBoolRecord _onBoolRecord;

	/// <summary>
	/// int32型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnInt32Record _onInt32Record;

	/// <summary>
	/// float型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnFloatRecord _onFloatRecord;

	/// <summary>
	/// FVector型データを読み込んだ際の配信
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Replay|Load")
	FOnVectorRecord _onVectorRecord;

	/// <summary>
	/// 読み書きどちらで動作するか
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Replay)
	EReplayMode _replayMode;

	/// <summary>
	/// 読み書きするログファイルの名前
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Replay)
	FString _replayLogName;

public:	
	// Sets default values for this component's properties
	UActionReplayComponent();

private:
	UFUNCTION()
	void OnBoolRecord(FString action, bool data);
	UFUNCTION()
	void OnInt32Record(FString action, int32 data);
	UFUNCTION()
	void OnFloatRecord(FString action, float data);
	UFUNCTION()
	void OnVectorRecord(FString action, FVector data);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveBool(const FString& action, bool data, bool force);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveInt32(const FString& action, int32 data, bool force);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveFloat(const FString& action, float data, bool force);

	UFUNCTION(BlueprintCallable, Category = "Replay|Save")
	void SaveVector(const FString& action, const FVector& data, bool force);
};
