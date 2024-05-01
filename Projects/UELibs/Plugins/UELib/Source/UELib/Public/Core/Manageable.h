// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/TickableObject.h"
#include "Core/Logger.h"
#include "Core/WaitFunction.h"
#include "Manageable.generated.h"


/// <summary>
/// Manageableの状態
/// </summary>
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

/// <summary>
/// 開始処理で渡すパラメータ
/// </summary>
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
	/// <summary>
	/// 現在の状態
	/// </summary>
	ManageCondition Condition;

	/// <summary>
	/// クラス名
	/// </summary>
	FString ClassName;

	/// <summary>
	/// 自身のColse時に合わせてCloseさせるManageableのリスト
	/// </summary>
	UPROPERTY()
	TArray<UManageable*> AutoCloseList;

protected:
	/// <summary>
	/// このManageableで使うロガー
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Manageable)
	ULogger* Logger;

	/// <summary>
	/// 待機が必要な開始処理のリスト
	/// </summary>
	UPROPERTY()
	TArray<UWaitableBase*> OpeningActor;
	/// <summary>
	/// 非同期での開始の待機タスクを保持する
	/// </summary>
	UE::Tasks::TTask<void> WaitOpeningTask;

public:
	UManageable();

private:
	/// <summary>
	/// 状態を変更する
	/// </summary>
	/// <param name="condition">変更先の状態</param>
	void SetCondition(ManageCondition Condition);

	/// <summary>
	/// 開始処理が終了したことの明示
	/// </summary>
	void OnOpenComplete();
public:

	// =================================

	/// <summary>
	/// 処理を開始する
	/// </summary>
	/// <param name="setting"></param>
	/// <returns>エラー無く開始できたか</returns>
	UFUNCTION(BlueprintCallable, Category = Manageable)
	bool Open(const FOpenSetting& setting);

	/// <summary>
	/// 処理を終了する
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Manageable)
	void Close();

	/// <summary>
	/// 処理を一時停止する
	/// </summary>
	/// <returns>成功したか</returns>
	UFUNCTION(BlueprintCallable, Category = Manageable)
	bool Pause();

	/// <summary>
	/// 処理を再開する
	/// </summary>
	/// <returns>成功したか</returns>
	UFUNCTION(BlueprintCallable, Category = Manageable)
	bool Resume();

	// ---------------------------
protected:
	/// <summary>
	/// 開始時イベント
	/// </summary>
	/// <returns>処理が成功したか</returns>
	UFUNCTION(BlueprintNativeEvent, Category = Manageable)
	bool OnOpen();
	virtual bool OnOpen_Implementation();

	/// <summary>
	/// 開始処理が完了したタイミング
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, Category = Manageable)
	void OnReady();
	virtual void OnReady_Implementation();

	/// <summary>
	/// 一時停止が呼ばれたタイミング
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintNativeEvent, Category = Manageable)
	bool OnPause();
	virtual bool OnPause_Implementation();

	/// <summary>
	/// 再開が呼ばれたタイミング
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintNativeEvent, Category = Manageable)
	bool OnResume();
	virtual bool OnResume_Implementation();

	/// <summary>
	/// 終了が呼ばれたタイミング
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, Category = Manageable)
	void OnClose();
	virtual void OnClose_Implementation();

	// --------------------------

	/// <summary>
	/// エラー発生状態にする
	/// </summary>
	UFUNCTION(BlueprintCallable, Category=Manageable)
	void SetFailed();

	/// <summary>
	/// 自身に合わせてCloseさせるManageableを登録する
	/// </summary>
	/// <param name="target"></param>
	UFUNCTION(BlueprintCallable, Category = Manageable)
	void SetAutoCloser(UManageable* Target);

	// ---------------------------
	FString GetClassName();

	// ---------------------------

	/// <summary>
	/// 非同期で実行する初期化関数を登録する
	/// </summary>
	/// <param name="openingAct"></param>
	void AddOpeningFunction(TFunction<bool()> OpeningAct);

	/// <summary>
	/// ラップされた待機処理を初期化処理として登録する
	/// </summary>
	/// <param name="openingAct"></param>
	UFUNCTION(BlueprintCallable, Category = Manageable)
	void AddOpeningAct(UWaitableBase* OpeningAct);

	// -----------------------------
	
	bool IsCreated() { return Condition == ManageCondition::Created; }
	bool IsOpen() { return Condition == ManageCondition::Open; }
	bool IsPause() { return Condition == ManageCondition::Pause; }
	bool IsFinished() { return Condition == ManageCondition::Finished; }

	/// <summary>
	/// Running状態か
	/// </summary>
	/// <returns>Runningの時のみtrue</returns>
	bool IsRunning() { return Condition == ManageCondition::Running; }
};
