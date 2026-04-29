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

PathResult dijkstra(const TransportNetwork& network, const std::string& start,
                    const std::string& destination, bool optimizeByTime);

std::vector<PathResult> generateCandidateJourneys(const TransportNetwork& network,
                                                  const std::string& start,
                                                  const std::string& destination,
                                                  int maxSegments,
                                                  int maxJourneys);

void rankJourneys(std::vector<PathResult>& journeys, int preferenceMode);

#endif
