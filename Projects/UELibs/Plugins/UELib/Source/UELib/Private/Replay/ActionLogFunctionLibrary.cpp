// Fill out your copyright notice in the Description page of Project Settings.


#include "Replay/ActionLogFunctionLibrary.h"
#include <Kismet/KismetSystemLibrary.h>

FString UActionLogFunctionLibrary::MakeLogFilePath(FString FileName)
{
	FString DirPath = UKismetSystemLibrary::GetProjectSavedDirectory() + TEXT("ActionRecord");
	if (!IFileManager::Get().DirectoryExists(*DirPath)) {
		IFileManager::Get().MakeDirectory(*DirPath, true);
	}
	return FPaths::Combine(DirPath, FileName);
}
