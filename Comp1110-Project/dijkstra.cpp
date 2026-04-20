#include "dijkstra.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <set>
#include <unordered_map>

namespace {
struct State {
    double distance;
    std::string station;

    bool operator>(const State& other) const {
        return distance > other.distance;
    }
};

PathResult buildPathResult(const std::vector<PathStep>& steps) {
    PathResult result = {false, {}, {}, 0, 0.0, 0};
    if (steps.empty()) {
        return result;
    }

    result.found = true;
    result.steps = steps;
    result.segmentCount = static_cast<int>(steps.size());
    result.stations.push_back(steps.front().from);

    for (const PathStep& step : steps) {
        result.stations.push_back(step.to);
        result.totalTime += step.duration;
        result.totalCost += step.cost;
    }

    return result;
}

bool sameRoute(const PathResult& left, const PathResult& right) {
    if (left.stations != right.stations) {
        return false;
    }

    if (left.steps.size() != right.steps.size()) {
        return false;
    }

    for (std::size_t i = 0; i < left.steps.size(); ++i) {
        if (left.steps[i].mode != right.steps[i].mode) {
            return false;
        }
    }

    return true;
}
}

PathResult dijkstra(const TransportNetwork& network, const std::string& start,
                    const std::string& destination, bool optimizeByTime) {
    PathResult result = {false, {}, {}, 0, 0.0, 0};

    if (!network.hasStation(start) || !network.hasStation(destination)) {
        return result;
    }

    std::unordered_map<std::string, double> dist;
    std::unordered_map<std::string, std::string> previousStation;
    std::unordered_map<std::string, PathStep> previousStep;

    for (const std::string& station : network.getStations()) {
        dist[station] = std::numeric_limits<double>::infinity();
    }

    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    dist[start] = 0.0;
    pq.push({0.0, start});

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        if (current.distance > dist[current.station]) {
            continue;
        }

        if (current.station == destination) {
            break;
        }

        const std::vector<Segment>& neighbors = network.getNeighbors(current.station);
        for (const Segment& segment : neighbors) {
            double weight = optimizeByTime ? static_cast<double>(segment.duration) : segment.cost;
            double newDistance = current.distance + weight;

            if (newDistance < dist[segment.destination]) {
                dist[segment.destination] = newDistance;
                previousStation[segment.destination] = current.station;
                previousStep[segment.destination] = {
                    current.station,
                    segment.destination,
                    segment.mode,
                    segment.duration,
                    segment.cost
                };
                pq.push({newDistance, segment.destination});
            }
        }
    }

    if (dist[destination] == std::numeric_limits<double>::infinity()) {
        return result;
    }

    std::vector<std::string> reversedStations;
    std::vector<PathStep> reversedSteps;
    std::string current = destination;

    reversedStations.push_back(current);
    while (current != start) {
        reversedSteps.push_back(previousStep[current]);
        current = previousStation[current];
        reversedStations.push_back(current);
    }

    std::reverse(reversedStations.begin(), reversedStations.end());
    std::reverse(reversedSteps.begin(), reversedSteps.end());

    result.found = true;
    result.stations = reversedStations;
    result.steps = reversedSteps;
    result.segmentCount = static_cast<int>(result.steps.size());

    for (const PathStep& step : result.steps) {
        result.totalTime += step.duration;
        result.totalCost += step.cost;
    }

    return result;
}

std::vector<PathResult> generateCandidateJourneys(const TransportNetwork& network,
                                                  const std::string& start,
                                                  const std::string& destination,
                                                  int maxSegments,
                                                  int maxJourneys) {
    std::vector<PathResult> journeys;
    if (!network.hasStation(start) || !network.hasStation(destination) || maxSegments <= 0 ||
        maxJourneys <= 0) {
        return journeys;
    }

    std::vector<PathStep> currentSteps;
    std::set<std::string> visited;
    visited.insert(start);

    std::function<void(const std::string&, int)> dfs = [&](const std::string& current,
                                                           int depthLeft) {
        if (static_cast<int>(journeys.size()) >= maxJourneys) {
            return;
        }

        if (current == destination && !currentSteps.empty()) {
            PathResult candidate = buildPathResult(currentSteps);
            bool duplicate = false;
            for (const PathResult& journey : journeys) {
                if (sameRoute(candidate, journey)) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate) {
                journeys.push_back(candidate);
            }
            return;
        }

        if (depthLeft == 0) {
            return;
        }

        const std::vector<Segment>& neighbors = network.getNeighbors(current);
        for (const Segment& segment : neighbors) {
            if (visited.count(segment.destination) > 0) {
                continue;
            }

            currentSteps.push_back({current, segment.destination, segment.mode, segment.duration,
                                    segment.cost});
            visited.insert(segment.destination);
            dfs(segment.destination, depthLeft - 1);
            visited.erase(segment.destination);
            currentSteps.pop_back();

            if (static_cast<int>(journeys.size()) >= maxJourneys) {
                return;
            }
        }
    };

    dfs(start, maxSegments);
    return journeys;
}

void rankJourneys(std::vector<PathResult>& journeys, int preferenceMode) {
    std::sort(journeys.begin(), journeys.end(), [preferenceMode](const PathResult& left,
                                                                 const PathResult& right) {
        if (preferenceMode == 1) {
            if (left.totalTime != right.totalTime) {
                return left.totalTime < right.totalTime;
            }
            if (left.totalCost != right.totalCost) {
                return left.totalCost < right.totalCost;
            }
            return left.segmentCount < right.segmentCount;
        }

        if (preferenceMode == 2) {
            if (left.totalCost != right.totalCost) {
                return left.totalCost < right.totalCost;
            }
            if (left.totalTime != right.totalTime) {
                return left.totalTime < right.totalTime;
            }
            return left.segmentCount < right.segmentCount;
        }

        if (left.segmentCount != right.segmentCount) {
            return left.segmentCount < right.segmentCount;
        }
        if (left.totalTime != right.totalTime) {
            return left.totalTime < right.totalTime;
        }
        return left.totalCost < right.totalCost;
    });
}
