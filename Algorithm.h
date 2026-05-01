#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>
#include <vector>

#include "TransportNetwork.h"

struct PathStep {
    std::string from;
    std::string to;
    std::string mode;
    int duration;
    double cost;
};

struct PathResult {
    bool found;
    std::vector<std::string> stations;
    std::vector<PathStep> steps;
    int totalTime;
    double totalCost;
    int segmentCount;
};

// Finds the single best path by either total time or total cost.
PathResult dijkstra(const TransportNetwork& network, const std::string& start,
                    const std::string& destination, bool optimizeByTime);

// Enumerates simple candidate journeys up to the given search limits.
std::vector<PathResult> generateCandidateJourneys(const TransportNetwork& network,
                                                  const std::string& start,
                                                  const std::string& destination,
                                                  int maxSegments,
                                                  int maxJourneys);

// Sorts journeys using the user's preference, with sensible tie breakers.
void rankJourneys(std::vector<PathResult>& journeys, int preferenceMode);

#endif
