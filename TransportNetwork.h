#ifndef TRANSPORT_NETWORK_H
#define TRANSPORT_NETWORK_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
struct Segment {
    std::string destination;
    std::string mode;
    int duration;
    double cost;
};
class TransportNetwork {
private:
    std::unordered_map<std::string, std::vector<Segment>> adjList;
    std::unordered_map<std::string, int> nameToId;
    std::string idToName[10];
    int nodeCount = 0;

public:








}