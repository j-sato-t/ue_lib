// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TickableObject.generated.h"


/**
*
*/
UCLASS(Blueprintable, BlueprintType)
	class UELIB_API UTickableObject : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

private:
	/// <summary>
	/// 実際に使われる動作するかの判定
	/// </summary>
	bool bIsTickable;

	/// <summary>
	/// 初期化処理が実行されたか
	/// </summary>
	bool bWasInit;

protected:


public:
	UTickableObject();

	// override
	void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	UWorld* GetWorld() const override;

protected:
	/// <summary>
	/// 周期処理実装用イベント
	/// </summary>
	/// <param name="DeltaTime">フレーム時間（秒）</param>
	UFUNCTION(BlueprintNativeEvent, Category = "UELib|Tickable", DisplayName = "Tick")
	void OnTick(float DeltaTime);
	virtual void OnTick_Implementation(float DeltaTime);

	/// <summary>
	/// そのオブジェクトの初回Tickで呼ばれるイベント
	/// </summary>
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UELib|Tickable")
	void Initialize();
	virtual void Initialize_Implementation();

	/// <summary>
	/// Tickを実行するか
	/// 初期値はfalseになっている
	/// </summary>
	/// <param name="newTickable">実行するならtrue</param>
	UFUNCTION(BlueprintCallable, Category = "UELib|Tickable")
	void SetTickable(bool bNewTickable);
};
