// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AStar.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FouttputData
{
    GENERATED_BODY()
        UPROPERTY(BlueprintReadWrite, Category = "Data")
        float lng;
    UPROPERTY(BlueprintReadWrite, Category = "Data")
        float lat;
    UPROPERTY(BlueprintReadWrite, Category = "Data")
        FString type;
    UPROPERTY(BlueprintReadWrite, Category = "Data")
        float distance;
    UPROPERTY(BlueprintReadWrite, Category = "Data")
        FString point_type;
    UPROPERTY(BlueprintReadWrite, Category = "Data")
        TArray<FName> neighbours;

};

UCLASS()
class RANDOM_API UAStar : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
        UFUNCTION(BlueprintCallable, Category = "myCategory")
        static TArray<FName> AStar(FName start_node_index, FName end_node_index, FouttputData start_node, FouttputData stop_node, TArray<FouttputData> items);

    static float distance(float lat1, float lat2, float long1, float long2);

    static float DegreesToRadians(float degrees);

    static TArray<float> get_H(float dest_lat, float dest_long, TArray<float> lat_arr, TArray<float> lng_arr);

    static TArray<FName> get_neighbours(FString index, TArray<FouttputData> nodes);

    static int FStringToInt(FString theString);
};
