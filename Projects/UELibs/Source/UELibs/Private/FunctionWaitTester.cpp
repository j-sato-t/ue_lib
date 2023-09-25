// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionWaitTester.h"
#include <chrono>
#include <thread>

// Sets default values
AFunctionWaitTester::AFunctionWaitTester()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SetActorTickEnabled(false);

}

// Called when the game starts or when spawned
void AFunctionWaitTester::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);

	_waitFunction = NewObject<UWaitFunction>(this);
	_waitFunction->SetFunction([this] {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		return true;
	});
	UE_LOG(LogTemp, Log, TEXT("start task"));
	_waitFunction->StartTask();
}

// Called every frame
void AFunctionWaitTester::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_waitFunction->IsSuccessed()) {
		SetActorTickEnabled(false);
		UE_LOG(LogTemp, Log, TEXT("end task"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("wait task"));
}

