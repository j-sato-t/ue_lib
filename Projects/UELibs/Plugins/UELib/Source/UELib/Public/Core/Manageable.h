// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/TickableObject.h"
#include "Core/Logger.h"
#include "Manageable.generated.h"


UENUM()
enum class ManageCondition : uint8 {
	// 生成された状態
	Created = 0,
	// 開始処理が呼ばれた状態
	Open,
	// 開始処理が終わり動作している状態
	Running,
	// 一時停止している状態
	Pause,
	// 再開処理中
	Resume,
	// 終了済
	Finished,

	// 何かしらエラーが発生した
	Failed,
};

USTRUCT(Blueprintable, BlueprintType)
struct FOpenSetting {
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, Category = Manageable)
	ELogLevel LogFilterLevel = ELogLevel::Info;
	UPROPERTY(BlueprintReadWrite, Category = Manageable)
	FString LogTag = TEXT("");
};

/**
*
*/
UCLASS(Blueprintable, BlueprintType)
	class UELIB_API UManageable : public UTickableObject
{
	GENERATED_BODY()
private:
	ManageCondition _condition;

	const char* _className;

	TArray<UManageable*> _autoCloesList;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Manageable)
	ULogger* _logger;

	// TODO: 開始処理を待つ方法
	// TODO: 終了をまとめてする

public:
	UManageable();

private:
	void SetCondition(ManageCondition condition);

	void OnOpenComplete();
public:

	// =================================
	UFUNCTION(BlueprintCallable, Category = Manageable)
	bool Open(const FOpenSetting& setting);
	UFUNCTION(BlueprintCallable, Category = Manageable)
	void Close();
	UFUNCTION(BlueprintCallable, Category = Manageable)
	bool Pause();
	UFUNCTION(BlueprintCallable, Category = Manageable)
	bool Resume();

	// ---------------------------
protected:
	UFUNCTION(BlueprintNativeEvent, Category = Manageable)
	bool OnOpen();
	virtual bool OnOpen_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = Manageable)
	void OnReady();
	virtual void OnReady_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = Manageable)
	bool OnPause();
	virtual bool OnPause_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = Manageable)
	bool OnResume();
	virtual bool OnResume_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = Manageable)
	void OnClose();
	virtual void OnClose_Implementation();

	// --------------------------

	UFUNCTION(BlueprintCallable, Category=Manageable)
	void SetFailed();

	UFUNCTION(BlueprintCallable, Category = Manageable)
	void SetAutoCloser(UManageable* target);
};
