// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Manageable.h"
#include <chrono>
#include <thread>

UManageable::UManageable()
{
	Condition = ManageCondition::Created;
	Logger = nullptr;
	ClassName = TEXT("");
}

void UManageable::SetCondition(ManageCondition condition)
{
	Condition = condition;
	if (Logger != nullptr) Logger->LogDebug(FString::Printf(TEXT("set condition: %s"), *UEnum::GetValueAsString(condition)));
}

void UManageable::OnOpenComplete()
{
	SetCondition(ManageCondition::Running);
	Logger->LogTrace(TEXT("Ready"));
	OnReady();
}

bool UManageable::Open(const FOpenSetting& setting)
{
	if (Condition != ManageCondition::Created) return false;

	Logger = NewObject<ULogger>(this);
	FString logTag = setting.LogTag.IsEmpty() ? GetClassName() : setting.LogTag;
	Logger->Setup(setting.LogFilterLevel, logTag, true);
	Logger->LogTrace(TEXT("Open"));

	if (!OnOpen()) {
		SetFailed();

		return false;
	}

	// 非同期の開始処理があるならそれを待つ
	if (!OpeningActor.IsEmpty()) {
		Logger->LogTrace(TEXT("start wait opening act"));
		WaitOpeningTask = UE::Tasks::Launch(UE_SOURCE_LOCATION, [this] {
			for (UWaitableBase* task : OpeningActor) {
				task->StartTask();
			}

			bool isEnd = false;
			while (true) {
				std::this_thread::sleep_for(std::chrono::milliseconds(8));

				isEnd = true;
				for (UWaitableBase* task : OpeningActor) {
					if (task->IsFailed()) {
						// 失敗時は即終了
						SetFailed();
						Logger->LogError(TEXT("fail opening"));
						return;
					}
					if (!task->IsSuccessed()) {
						isEnd = false;
						break;
					}
				}
				if (isEnd) {
					Logger->LogTrace(TEXT("end wait opening act"));
					break;
				}
			}

			OnOpenComplete();
		});
	}
	// なければ開始処理終了とする
	else
	{
		OnOpenComplete();
	}

	return true;
}

void UManageable::Close()
{
	if (Condition == ManageCondition::Created || Condition == ManageCondition::Finished)
	{
		Logger->LogError(TEXT("call close but not running"));
		return;
	}
	Logger->LogTrace(TEXT("Close"));

	SetCondition(ManageCondition::Finished);

	SetTickable(false);

	OnClose();

	// 自動終了
	for (UManageable* closeTarget : AutoCloseList)
	{
		closeTarget->Close();
	}
}

bool UManageable::Pause()
{
	if (Condition != ManageCondition::Running) return false;
	Logger->LogTrace(TEXT("Pause"));
	SetCondition(ManageCondition::Pause);

	return OnPause();
}

bool UManageable::Resume()
{
	if (Condition != ManageCondition::Pause) return false;
	Logger->LogTrace(TEXT("Resume"));
	SetCondition(ManageCondition::Running);
	if (OnResume()) 
	{
		return true;
	}
	else
	{
		SetFailed();
		return false;
	}
}

bool UManageable::OnOpen_Implementation()
{
	return true;
}

void UManageable::OnReady_Implementation()
{
}

bool UManageable::OnPause_Implementation()
{
	return true;
}

bool UManageable::OnResume_Implementation()
{
	return true;
}

void UManageable::OnClose_Implementation()
{
}

void UManageable::SetFailed()
{
	SetCondition(ManageCondition::Failed);
	if (Logger != nullptr) Logger->LogError(TEXT("on set failed"));
}

void UManageable::SetAutoCloser(UManageable* target)
{
	if (target == nullptr) return;
	AutoCloseList.Add(target);
}

FString UManageable::GetClassName()
{
	if (!ClassName.IsEmpty()) return ClassName;
	ClassName = GetClass()->GetName();
	return ClassName;
}

void UManageable::AddOpeningFunction(TFunction<bool()> openingAct)
{
	UWaitFunction* task = NewObject<UWaitFunction>(this);
	task->SetFunction(openingAct);
	OpeningActor.Add(task);
}

void UManageable::AddOpeningAct(UWaitableBase* openingAct)
{
	if (openingAct == nullptr) {
		Logger->LogWarning(TEXT("openingAct is null"));
		return;
	}
	OpeningActor.Add(openingAct);
}
