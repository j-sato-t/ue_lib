// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrayTester.h"

// Sets default values
AArrayTester::AArrayTester()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArrayTester::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < MaxNum; i++) {
		StackTest.Push(i + 1);
	}

	for (int32 i = 0; i < MaxNum; i++) {
		UE_LOG(LogTemp, Log, TEXT("pop data: %d"), StackTest.Pop());
	}
}

// Called every frame
void AArrayTester::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

