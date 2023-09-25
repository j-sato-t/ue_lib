// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/WaitFunction.h"
#include "FunctionWaitTester.generated.h"

UCLASS()
class UELIBS_API AFunctionWaitTester : public AActor
{
	GENERATED_BODY()

private:
	UWaitFunction* _waitFunction;
	
public:	
	// Sets default values for this actor's properties
	AFunctionWaitTester();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
