// Fill out your copyright notice in the Description page of Project Settings.


#include "Replay/ActionLogReader.h"
#include "Replay/ActionLogFunctionLibrary.h"

bool UActionLogReader::OpenFile(FString FileName)
{
    FString filePath = UActionLogFunctionLibrary::MakeLogFilePath(FileName);

    FilePointer = fopen(TCHAR_TO_ANSI(*filePath), "r");

    return FilePointer != NULL;
}

bool UActionLogReader::ReadLine(FString& OutLine)
{
    if (FilePointer == NULL) return false;

    char* ret = fgets(LineBuff, MAX_LINE_LENGTH, FilePointer);
    if (ret == NULL) return false;

    OutLine = FString(ANSI_TO_TCHAR(LineBuff));

    return true;
}

void UActionLogReader::BeginDestroy()
{
    if (FilePointer != NULL) {
        fclose(FilePointer);
        FilePointer = NULL;
    }

    Super::BeginDestroy();
}
