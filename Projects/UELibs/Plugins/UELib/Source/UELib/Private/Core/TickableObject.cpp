// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TickableObject.h"

UTickableObject::UTickableObject() : Super()
{
	_bIsTickable = false;
	_bWasInit = false;
}

void UTickableObject::Tick(float DeltaTime)
{
	if (!_bWasInit)
	{
		_bWasInit = true;
		Initialize();
	}
	OnTick(DeltaTime);
}

bool UTickableObject::IsTickable() const
{
	return _bIsTickable;
}

bool UTickableObject::IsTickableInEditor() const
{
	return false;
}

bool UTickableObject::IsTickableWhenPaused() const
{
	return false;
}

TStatId UTickableObject::GetStatId() const
{
	return TStatId();
}

UWorld* UTickableObject::GetWorld() const
{
	return GetOuter()->GetWorld();
}

void UTickableObject::OnTick_Implementation(float DeltaTime)
{
}

void UTickableObject::Initialize_Implementation()
{
}

void UTickableObject::SetTickable(bool newTickable)
{
	_bIsTickable = newTickable;
}
