#include "TransportNetwork.h"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace {
std::string trim(const std::string& text) {
    std::size_t start = text.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    std::size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}
}

void TransportNetwork::addBidirectionalSegment(const std::string& stop1, const std::string& stop2,
                                               const std::string& mode, int duration, double cost) {
    adjList[stop1].push_back({stop2, mode, duration, cost});
    adjList[stop2].push_back({stop1, mode, duration, cost});
}

void TransportNetwork::addDirectedSegment(const std::string& from, const std::string& to,
                                          const std::string& mode, int duration, double cost) {
    adjList[from].push_back({to, mode, duration, cost});
}

void TransportNetwork::clear() {
    adjList.clear();
}

void TransportNetwork::buildMap() {
    clear();

    addBidirectionalSegment("Sham Shui Po", "Mong Kok", "MTR", 4, 4.8);
    addBidirectionalSegment("Sham Shui Po", "Mong Kok", "Walk", 20, 0.0);

    addBidirectionalSegment("Mong Kok", "Tsim Sha Tsui", "MTR", 6, 5.7);
    addBidirectionalSegment("Mong Kok", "Tsim Sha Tsui", "Bus", 15, 5.4);

    addBidirectionalSegment("Mong Kok", "Kowloon Tong", "MTR", 6, 5.7);
    addBidirectionalSegment("Kowloon Tong", "Hung Hom", "MTR", 7, 6.5);
    addBidirectionalSegment("Kowloon Tong", "Wong Tai Sin", "MTR", 5, 4.8);
    addBidirectionalSegment("Wong Tai Sin", "Kwun Tong", "MTR", 10, 7.1);

    addBidirectionalSegment("Mong Kok", "Kwun Tong", "Red Minibus", 25, 12.0);
    addBidirectionalSegment("Tsim Sha Tsui", "Hung Hom", "Walk", 15, 0.0);

    addBidirectionalSegment("Tsim Sha Tsui", "Central", "Ferry", 10, 4.0);
    addBidirectionalSegment("Tsim Sha Tsui", "Admiralty", "MTR", 5, 10.5);
    addBidirectionalSegment("Hung Hom", "Admiralty", "MTR", 5, 10.5);
    addBidirectionalSegment("Hung Hom", "HKU", "Cross Bus", 40, 16.0);

    addBidirectionalSegment("Central", "Admiralty", "MTR", 2, 4.8);
    addBidirectionalSegment("Central", "Admiralty", "Walk", 15, 0.0);
    addBidirectionalSegment("Central", "HKU", "MTR", 7, 5.7);
    addBidirectionalSegment("Central", "HKU", "Bus", 15, 6.2);
    addBidirectionalSegment("Admiralty", "HKU", "MTR", 8, 5.7);
}

bool TransportNetwork::loadFromFile(const std::string& filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        return false;
    }

    std::unordered_map<std::string, std::vector<Segment>> newAdjList;
    std::string line;

    while (std::getline(input, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::stringstream ss(line);
        std::string stop1;
        std::string stop2;
        std::string mode;
        std::string durationText;
        std::string costText;

        if (!std::getline(ss, stop1, ',')) {
            continue;
        }
        if (!std::getline(ss, stop2, ',')) {
            continue;
        }
        if (!std::getline(ss, mode, ',')) {
            continue;
        }
        if (!std::getline(ss, durationText, ',')) {
            continue;
        }
        if (!std::getline(ss, costText)) {
            continue;
        }

        stop1 = trim(stop1);
        stop2 = trim(stop2);
        mode = trim(mode);
        durationText = trim(durationText);
        costText = trim(costText);

        if (stop1.empty() || stop2.empty() || mode.empty()) {
            continue;
        }

        int duration = 0;
        double cost = 0.0;
        try {
            duration = std::stoi(durationText);
            cost = std::stod(costText);
        } catch (...) {
            continue;
        }

        newAdjList[stop1].push_back({stop2, mode, duration, cost});
        newAdjList[stop2].push_back({stop1, mode, duration, cost});
    }

    if (newAdjList.empty()) {
        return false;
    }

    adjList = newAdjList;
    return true;
}

bool TransportNetwork::hasStation(const std::string& station) const {
    return adjList.find(station) != adjList.end();
}

const std::vector<Segment>& TransportNetwork::getNeighbors(const std::string& station) const {
    static const std::vector<Segment> empty;
    auto it = adjList.find(station);
    if (it == adjList.end()) {
        return empty;
    }
    return it->second;
}

std::vector<std::string> TransportNetwork::getStations() const {
    std::vector<std::string> stations;
    for (const auto& entry : adjList) {
        stations.push_back(entry.first);
    }
    std::sort(stations.begin(), stations.end());
    return stations;
}

std::size_t TransportNetwork::getStopCount() const {
    return adjList.size();
}

std::size_t TransportNetwork::getSegmentCount() const {
    std::size_t count = 0;
    for (const auto& entry : adjList) {
        count += entry.second.size();
    }
    return count;
}
