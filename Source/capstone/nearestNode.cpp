// Fill out your copyright notice in the Description page of Project Settings.


#include "nearestNode.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <map>  
#include <sstream>
#include "Algo/Reverse.h"

float UnearestNode::distance(float lat1, float lat2, float long1, float long2)
{
    lat1 = DegreesToRadians(lat1);
    lat2 = DegreesToRadians(lat2);
    long1 = DegreesToRadians(long1);
    long2 = DegreesToRadians(long2);

    float dlon = long2 - long1;
    float dlat = lat2 - lat1;

    float a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
    float c = 2 * asin(sqrt(a));
    float r = 6371;

    return c * r;
}

float UnearestNode::DegreesToRadians(float degrees)
{
    return degrees * (PI / 180);
}

int UnearestNode::FStringToInt(FString theString)
{
    return FCString::Atoi(*theString);
}


// this algo here will return the nearest nodes by checking which is the nearest to the current point
int UnearestNode::algo(float lat, float lon, TArray<FouttputData> nodes, TArray<FString> nodes_arr, TArray<float> lng_arr, TArray<float> lat_arr, TArray<float> distance_arr, TArray<FString> point_type_arr, TMap<FString, bool> checkBoxes_map)
{
    float shortest_distance = 99999999.0;
    int return_node;
    for (int i = 0; i < nodes.Num(); i++)
    {
        bool IsSelected = checkConstraints(point_type_arr[i], checkBoxes_map);
        if (distance(lat, lat_arr[i], lon, lng_arr[i]) < shortest_distance && IsSelected)
        {
            shortest_distance = distance(lat, lat_arr[i], lon, lng_arr[i]);
            return_node = i;
        }
    }

    return return_node;
}

bool UnearestNode::checkConstraints(FString keyValue, TMap<FString, bool> checkBox)
{
    bool checkIfKeyExist = checkBox.Contains(keyValue);
    if (checkIfKeyExist) {
        UE_LOG(LogTemp, Warning, TEXT("Key exist %s"), *keyValue);
        return checkBox[keyValue];
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Key does not exist %s"), *keyValue);
        return false;
    }

}

void UnearestNode::getNearestNode(TMap<FString, bool> checkBoxes_map, float start_lat, float start_long, float dest_lat, float dest_long, TArray<FouttputData> nodes, FName& start_node_index, FName& stop_node_index, FouttputData& start_node, FouttputData& stop_node)
{
    TArray<FString> nodes_arr;
    TArray<float> lng_arr;
    TArray<float> lat_arr;
    TArray<FString> type_arr;
    TArray<float> distance_arr;
    TArray<FString> point_type_arr;
    

    for (int i = 0; i < nodes.Num(); i++)
    {
        nodes_arr.Add(FString::FromInt(i + 2)); // unvisited nodes
        lng_arr.Add(nodes[i].lng);
        lat_arr.Add(nodes[i].lat);
        type_arr.Add(nodes[i].type);
        distance_arr.Add(nodes[i].lng);
        point_type_arr.Add(nodes[i].point_type);
    }

    int int_start_node_index = algo(start_lat, start_long, nodes,nodes_arr,lng_arr,lat_arr,distance_arr, point_type_arr, checkBoxes_map);
    int int_stop_node_index = algo(dest_lat, dest_long, nodes, nodes_arr, lng_arr, lat_arr, distance_arr, point_type_arr, checkBoxes_map);

    FString fstring_start_node_index = nodes_arr[int_start_node_index];
    FString fstring_stop_node_index = nodes_arr[int_stop_node_index];
    
    start_node_index = FName(*fstring_start_node_index);
    stop_node_index = FName(*fstring_stop_node_index);

    start_node = nodes[int_start_node_index];
    stop_node = nodes[int_stop_node_index];

}
