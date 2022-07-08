// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StringToJson.generated.h"

/**
 *
 */
UCLASS()
class CAPSTONE_API UStringToJson : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
        UFUNCTION(BlueprintCallable, Category = "myCategory")
        static bool SaveStringToFile(FString SaveDirectory, FString FileName, FString SaveText, bool AllowOverwriting);
};