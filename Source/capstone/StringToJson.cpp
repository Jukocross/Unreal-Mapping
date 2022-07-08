// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "StringToJson.h"

bool UStringToJson::SaveStringToFile(FString SaveDirectory, FString FileName, FString SaveText, bool AllowOverwriting = false)
{
    SaveDirectory += "\\";
    SaveDirectory += FileName;

    if (!AllowOverwriting)
    {
        if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
        {
            return false;
        }
    }

    return FFileHelper::SaveStringToFile(SaveText, *SaveDirectory);


}