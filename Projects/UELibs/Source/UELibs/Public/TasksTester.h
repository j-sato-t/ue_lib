// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tasks/Task.h"
#include "TasksTester.generated.h"



UCLASS(Blueprintable, BlueprintType)
class UELIBS_API ATasksTester : public AActor
{
	GENERATED_BODY()
	
private:
	UE::Tasks::TTask<void> _testTask;

public:	
	// Sets default values for this actor's properties
	ATasksTester();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
