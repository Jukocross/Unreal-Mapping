// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AStar.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "nearestNode.generated.h"

/**
 * 
 */

UCLASS()
class CAPSTONE_API UnearestNode : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
        UFUNCTION(BlueprintCallable, Category = "myCategory")
        static void getNearestNode(TMap<FString, bool> checkBoxes_map, float start_lat, float start_long, float dest_lat, float dest_long, TArray<FouttputData> nodes, FName& start_node_index, FName& stop_node_index, FouttputData& start_node, FouttputData& stop_node);

    static float distance(float lat1, float lat2, float long1, float long2);

    static float DegreesToRadians(float degrees);

    static int FStringToInt(FString theString);

    static int algo(float lat, float lon, TArray<FouttputData> nodes, TArray<FString> nodes_arr, TArray<float> lng_arr, TArray<float> lat_arr, TArray<float> distance_arr, TArray<FString> point_type_arr, TMap<FString, bool> checkBoxes_map);

    static bool checkConstraints(FString keyValue, TMap<FString, bool> checkBox);
};