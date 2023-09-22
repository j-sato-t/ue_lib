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

	bool bWasInit;

protected:


public:
	UTickableObject();

	// override
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	UWorld* GetWorld() const override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = Tick, DisplayName = "Tick")
		void OnTick(float DeltaTime);
	virtual void OnTick_Implementation(float DeltaTime);

	/// <summary>
	/// そのオブジェクトの初回Tickで呼ばれる
	/// </summary>
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Tick)
		void Initialize();
	virtual void Initialize_Implementation();

	UFUNCTION(BlueprintCallable, Category = Tick)
		void SetTickable(bool newTickable);
};
