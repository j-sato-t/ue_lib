// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Manageable.h"

UManageable::UManageable() : _className(__func__)
{
	_condition = ManageCondition::Created;
	_logger = nullptr;
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
	FString logTag = setting.LogTag.IsEmpty() ? _className : setting.LogTag;
	_logger->Setup(setting.LogFilterLevel, logTag, true);
	_logger->LogTrace(TEXT("Open"));

	if (!OnOpen()) {
		SetFailed();

		return false;
	}

	// TODO: 非同期の開始処理があるならそれを待つ
	// if () {}

	// なければ開始処理終了とする
	// else
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

	// TODO: 自動終了
	for (UManageable* closeTarget : _autoCloesList)
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
	_autoCloesList.Add(target);
}
