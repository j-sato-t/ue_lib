// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/TickableObject.h"
#include "WaitableBase.generated.h"

/// <summary>
/// タスクの状態
/// </summary>
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
	/// <summary>
	/// 現在の状態
	/// </summary>
	UPROPERTY()
	ETaskStatus Status = ETaskStatus::Running;
protected:
	/// <summary>
	/// 完了したことをセットする
	/// </summary>
	UFUNCTION(BlueprintCallable, Category=Task)
	void SetFinish();

	/// <summary>
	/// エラーになったことをセットする
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "UELib|Task")
	void SetFailed();
public:

	/// <summary>
	/// タスクを開始する
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UELib|Task")
	void StartTask();
	virtual void StartTask_Implementation();

	UFUNCTION(BlueprintPure, Category = "UELib|Task")
	bool IsSuccessed() { return Status == ETaskStatus::Finished; }
	UFUNCTION(BlueprintPure, Category = "UELib|Task")
	bool IsFailed() { return Status == ETaskStatus::Failed; }
};
