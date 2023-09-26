// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/WaitableBase.h"
#include "Tasks/Task.h"
#include "WaitFunction.generated.h"

DECLARE_DELEGATE_RetVal(bool, FWaitFunction);

/**
 * 
 */
UCLASS()
class UELIB_API UWaitFunction : public UWaitableBase
{
	GENERATED_BODY()
private:
	/// <summary>
	/// 実行する関数のデリゲート
	/// </summary>
	FWaitFunction _waitFunction;

	/// <summary>
	/// タスク本体
	/// </summary>
	UE::Tasks::TTask<void> _taskBody;

protected:
	virtual void StartTask_Implementation() override;

public:
	/// <summary>
	/// 実行する関数をセットする
	/// </summary>
	/// <param name="taskFunction">引数無しでboolを返す関数</param>
	void SetFunction(TFunction<bool()> taskFunction);
};
