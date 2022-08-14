// Fill out your copyright notice in the Description page of Project Settings.

#include "AStar.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <map>  
#include <sstream>
#include "Algo/Reverse.h"

float UAStar::distance(float lat1, float lat2, float long1, float long2)
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

float UAStar::DegreesToRadians(float degrees)
{
    return degrees * (PI / 180);
}

TArray<float> UAStar::get_H(float dest_lat, float dest_long, TArray<float> lat_arr, TArray<float> lng_arr)
{
    TArray<float> H;
    for (int i = 0; i < lat_arr.Num(); i++)
    {
        H.Add(distance(lat_arr[i], dest_lat, lng_arr[i], dest_long));
    }

    return H;
}

int UAStar::FStringToInt(FString theString)
{
    return FCString::Atoi(*theString);
}

TArray<FName> UAStar::get_neighbours(FString index, TArray<FouttputData> nodes)
{
    return nodes[FStringToInt(index) - 2].neighbours;
    //return nodes[0].neighbours;
}

bool UAStar::checkConstraints(FString keyValue, TMap<FString, bool> checkBox)
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

TArray<FName> UAStar::AStar(TMap<FString, bool> checkBoxes_map, FName start_node_index, FName end_node_index, FouttputData start_node, FouttputData stop_node, TArray<FouttputData> nodes)
{
    //FString toReturn = items[0].ToString();
    TArray<FString> nodes_arr;
    TArray<float> lng_arr;
    TArray<float> lat_arr;
    TArray<FString> type_arr;
    TArray<float> distance_arr;
    TArray<FString> point_type_arr;
    bool flagFound = false;

    // IMPORTANT HERE

    for (int i = 0; i < nodes.Num(); i++)
    {
        nodes_arr.Add(FString::FromInt(i + 2)); // unvisited nodes
        lng_arr.Add(nodes[i].lng);
        lat_arr.Add(nodes[i].lat);
        type_arr.Add(nodes[i].type);
        distance_arr.Add(nodes[i].lng);
        point_type_arr.Add(nodes[i].point_type);
    }
    TArray<float> H = get_H(stop_node.lat, stop_node.lng, lat_arr, lng_arr); //will store all the heurstic values

    // We'll use this dict to save the cost of visiting each node and update it as we move along the graph
    TMap<FString, float> shortest_path;
    // We'll use this dict to save the shortest known path to a node found so far
    TMap<FString, FString> previous_nodes;

    // We'll use max_value to initialize the "infinity" value of the unvisited nodes   
    float max_value = 9999;
    for (int i = 0; i < nodes_arr.Num(); i++)
    {
        shortest_path.Add(nodes_arr[i], max_value);
    }
    // However, we initialize the starting node's value with 0   
    shortest_path.Add(start_node_index.ToString(), 0);

    // The algorithm executes until we visit all nodes
    FString current_min_node;
    UE_LOG(LogTemp, Warning, TEXT("Before loop"));
    while (nodes_arr.Num() > 0)
    {
        current_min_node = TEXT("NULL");
        for (int i = 0; i < nodes_arr.Num(); i++) // Iterate over the nodes
        {
            if (current_min_node.Equals(TEXT("NULL")))
            {
                current_min_node = nodes_arr[i];
            }
            else if (shortest_path[nodes_arr[i]] < shortest_path[current_min_node])
            {
                current_min_node = nodes_arr[i];
            }
        }
        // UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *current_min_node);
        // The code block below retrieves the current node's neighbors and updates their distances
        TArray<FName> neighbours = get_neighbours(current_min_node, nodes);
        int count;
        for (int i = 0; i < neighbours.Num(); i++)
        {
            count++;
            FString neighbour_string = neighbours[i].ToString();
            int neighbour_value = FStringToInt(neighbour_string) - 2;
            if ((neighbour_value) >= 0)
            {
                // UE_LOG(LogTemp, Warning, TEXT("Output: %d"), neighbour_value);
                float tentative_value = shortest_path[current_min_node] + distance(lat_arr[FStringToInt(current_min_node)], lat_arr[neighbour_value], lng_arr[FStringToInt(current_min_node)], lng_arr[neighbour_value]) + H[neighbour_value];
                FString new_neighbour = neighbours[i].ToString();
                bool IsSelected = checkConstraints(point_type_arr[FStringToInt(current_min_node)], checkBoxes_map);
                if (tentative_value < shortest_path[new_neighbour] && IsSelected)
                {
                    shortest_path[new_neighbour] = tentative_value;

                    // We also update the best path to the current node
                    if (previous_nodes.Contains(new_neighbour))
                    {
                        previous_nodes[new_neighbour] = current_min_node;
                    }
                    else
                    {
                        previous_nodes.Add(new_neighbour, current_min_node);
                    }
                }
            }
            // UE_LOG(LogTemp, Warning, TEXT("Output: %d %s"), neighbour_value,*current_min_node);

        }
        UE_LOG(LogTemp, Warning, TEXT("Counters: %d"), count);
        // After visiting its neighbors, we mark the node as "visited"
        nodes_arr.Remove(current_min_node);
        if (current_min_node.Equals(end_node_index.ToString()))
        {
            flagFound = true;
            UE_LOG(LogTemp, Warning, TEXT("Counters if true: %d"), count);
            break;
        }
    }

    if (flagFound) {
        // Post processing to get the final path
        TArray<FName> path;
        FString node = end_node_index.ToString();
        while (node != start_node_index.ToString())
        {
            path.Add(FName(*node));
            if (previous_nodes.Contains(node)) // Added check for node
            {
                node = previous_nodes[node];
            }
            else
            {
                TArray<FName> emptyPath;
                return emptyPath;
            }
            // UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *previous_nodes[node]);
        }

        // Add the start node manually
        path.Add(start_node_index);

        // Reverse the path
        Algo::Reverse(path);


        // DEBUG EXAMPLE
        UE_LOG(LogTemp, Warning, TEXT("Output: %s"), previous_nodes.Contains(end_node_index.ToString()) ? TEXT("TRUE") : TEXT("False"));


        return path;
        // return( FString::SanitizeFloat(H[0]));
        // return final_node;
        // return FString::FromInt(shortest_path[start_node_index.ToString()]);
        // return nodes[0].neighbours[0].ToString();
        // return (FString::Printf(TEXT("Hello Unreal, %d + %d = %d"),a,b,foo));
    }
    else {
        TArray<FName> path;
        return path;
    }
}
