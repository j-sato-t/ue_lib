// Fill out your copyright notice in the Description page of Project Settings.


#include "ManageableTest.h"
#include <chrono>
#include <thread>

bool UManageableTest::OnOpen_Implementation()
{
	AddOpeningFunction([this] {
		Logger->LogInfo(TEXT("start opening function"));
		std::this_thread::sleep_for(std::chrono::seconds(3));
		Logger->LogInfo(TEXT("end opening function"));
		return true;
	});
	return true;
}
