// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TickableObject.h"

UTickableObject::UTickableObject() : Super()
{
	bIsTickable = false;
	bWasInit = false;
}

void UTickableObject::Tick(float DeltaTime)
{
	if (!bWasInit)
	{
		bWasInit = true;
		Initialize();
	}
	OnTick(DeltaTime);
}

bool UTickableObject::IsTickable() const
{
	return bIsTickable;
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
	bIsTickable = newTickable;
}
