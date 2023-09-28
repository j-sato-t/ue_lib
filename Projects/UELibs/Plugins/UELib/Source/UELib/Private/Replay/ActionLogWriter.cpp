// Fill out your copyright notice in the Description page of Project Settings.


#include "Replay/ActionLogWriter.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Replay/ActionLogFunctionLibrary.h"

UActionLogWriter::UActionLogWriter() : Super()
{
	Buffer = {};
}

void UActionLogWriter::Setting(FString FileName, int32 BufferSize)
{
	FilePath = UActionLogFunctionLibrary::MakeLogFilePath(FileName);

	// すでにあるなら消す
	IFileManager::Get().Delete(*FilePath, true, false, true);

	BufferMaxSize = BufferSize;
}

void UActionLogWriter::AddLogString(FString LogString)
{
	Buffer.Add(LogString);

	if (Buffer.Num() > BufferMaxSize) {
		WriteToFile();
	}
}

bool UActionLogWriter::WriteToFile()
{
	if (Buffer.Num() <= 0) return false;

	bool ret = FFileHelper::SaveStringArrayToFile(Buffer, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	Buffer.Empty();

	return ret;
}

void UActionLogWriter::BeginDestroy()
{
	WriteToFile();
	Super::BeginDestroy();
}
