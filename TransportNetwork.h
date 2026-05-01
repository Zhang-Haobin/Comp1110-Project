#ifndef TRANSPORT_NETWORK_H
#define TRANSPORT_NETWORK_H

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

struct Segment {
    std::string destination;
    std::string mode;
    int duration;
    double cost;
};

// Stores the transport map as an adjacency list. Each stop maps to the
// outgoing travel segments that can be used from that stop.
class TransportNetwork {
public:
    void addBidirectionalSegment(const std::string& stop1, const std::string& stop2,
                                 const std::string& mode, int duration, double cost);
    void addDirectedSegment(const std::string& from, const std::string& to,
                            const std::string& mode, int duration, double cost);
    void clear();
    void buildMap();
    bool loadFromFile(const std::string& filename, std::string* errorMessage = nullptr);
    bool hasStation(const std::string& station) const;
    const std::vector<Segment>& getNeighbors(const std::string& station) const;
    std::vector<std::string> getStations() const;
    std::size_t getStopCount() const;
    std::size_t getSegmentCount() const;

private:
    std::unordered_map<std::string, std::vector<Segment>> adjList;
    // Counts logical connections once, even though bidirectional routes are
    // stored as two adjacency-list entries.
    std::size_t logicalSegmentCount = 0;
};

#endif
