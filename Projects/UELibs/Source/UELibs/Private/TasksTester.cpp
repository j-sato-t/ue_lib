// Fill out your copyright notice in the Description page of Project Settings.


#include "TasksTester.h"
#include <chrono>
#include <thread>

// Sets default values
ATasksTester::ATasksTester()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATasksTester::BeginPlay()
{
	Super::BeginPlay();
	
	_testTask = UE::Tasks::Launch(UE_SOURCE_LOCATION, [] {
		UE_LOG(LogTemp, Log, TEXT("task body")); 
		std::this_thread::sleep_for(std::chrono::seconds(1));
	});
}

// Called every frame
void ATasksTester::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_testTask.IsCompleted()) {
		UE_LOG(LogTemp, Log, TEXT("checked completed"));
		SetActorTickEnabled(false);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("waiting"));
	}
}

