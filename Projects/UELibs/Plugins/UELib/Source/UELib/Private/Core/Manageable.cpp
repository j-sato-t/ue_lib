// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Manageable.h"
#include <chrono>
#include <thread>

UManageable::UManageable()
{
	_condition = ManageCondition::Created;
	_logger = nullptr;
	_className = TEXT("");
}

void UManageable::SetCondition(ManageCondition condition)
{
	_condition = condition;
	if (_logger != nullptr) _logger->LogDebug(FString::Printf(TEXT("set condition: %s"), *UEnum::GetValueAsString(condition)));
}

void UManageable::OnOpenComplete()
{
	SetCondition(ManageCondition::Running);
	_logger->LogTrace(TEXT("Ready"));
	OnReady();
}

bool UManageable::Open(const FOpenSetting& setting)
{
	if (_condition != ManageCondition::Created) return false;

	_logger = NewObject<ULogger>(this);
	FString logTag = setting.LogTag.IsEmpty() ? GetClassName() : setting.LogTag;
	_logger->Setup(setting.LogFilterLevel, logTag, true);
	_logger->LogTrace(TEXT("Open"));

	if (!OnOpen()) {
		SetFailed();

		return false;
	}

	// 非同期の開始処理があるならそれを待つ
	if (!_openingActor.IsEmpty()) {
		_logger->LogTrace(TEXT("start wait opening act"));
		_waitOpeningTask = UE::Tasks::Launch(UE_SOURCE_LOCATION, [this] {
			for (UWaitableBase* task : _openingActor) {
				task->StartTask();
			}

			bool isEnd = false;
			while (true) {
				std::this_thread::sleep_for(std::chrono::milliseconds(8));

				isEnd = true;
				for (UWaitableBase* task : _openingActor) {
					if (task->IsFailed()) {
						// 失敗時は即終了
						SetFailed();
						_logger->LogError(TEXT("fail opening"));
						return;
					}
					if (!task->IsSuccessed()) {
						isEnd = false;
						break;
					}
				}
				if (isEnd) {
					_logger->LogTrace(TEXT("end wait opening act"));
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
	if (_condition == ManageCondition::Created || _condition == ManageCondition::Finished)
	{
		_logger->LogError(TEXT("call close but not running"));
		return;
	}
	_logger->LogTrace(TEXT("Close"));

	SetCondition(ManageCondition::Finished);

	SetTickable(false);

	OnClose();

	// 自動終了
	for (UManageable* closeTarget : _autoCloseList)
	{
		closeTarget->Close();
	}
}

bool UManageable::Pause()
{
	if (_condition != ManageCondition::Running) return false;
	_logger->LogTrace(TEXT("Pause"));
	SetCondition(ManageCondition::Pause);

	return OnPause();
}

bool UManageable::Resume()
{
	if (_condition != ManageCondition::Pause) return false;
	_logger->LogTrace(TEXT("Resume"));
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
	if (_logger != nullptr) _logger->LogError(TEXT("on set failed"));
}

void UManageable::SetAutoCloser(UManageable* target)
{
	if (target == nullptr) return;
	_autoCloseList.Add(target);
}

FString UManageable::GetClassName()
{
	if (!_className.IsEmpty()) return _className;
	_className = GetClass()->GetName();
	return _className;
}

void UManageable::AddOpeningFunction(TFunction<bool()> openingAct)
{
	UWaitFunction* task = NewObject<UWaitFunction>(this);
	task->SetFunction(openingAct);
	_openingActor.Add(task);
}

void UManageable::AddOpeningAct(UWaitableBase* openingAct)
{
	if (openingAct == nullptr) {
		_logger->LogWarning(TEXT("openingAct is null"));
		return;
	}
	_openingActor.Add(openingAct);
}
