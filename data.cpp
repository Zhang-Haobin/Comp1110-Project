
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

public:
    void addBidirectionalSegment(const std::string& stop1, const std::string& stop2, 
                                 const std::string& mode, int duration, double cost) {
        // 正向：stop1 -> stop2
        adjList[stop1].push_back({stop2, mode, duration, cost});
        // 反向：stop2 -> stop1
        adjList[stop2].push_back({stop1, mode, duration, cost});
    }


    void addDirectedSegment(const std::string& from, const std::string& to, 
                            const std::string& mode, int duration, double cost) {
        adjList[from].push_back({to, mode, duration, cost});
    }

    void buildMap() {
      
        addBidirectionalSegment("Sham Shui Po", "Mong Kok", "MTR", 4, 4.8);
        addBidirectionalSegment("Sham Shui Po", "Mong Kok", "Walk", 20, 0.0);
        
        addBidirectionalSegment("Mong Kok", "Tsim Sha Tsui", "MTR", 6, 5.7);
        addBidirectionalSegment("Mong Kok", "Tsim Sha Tsui", "Bus", 15, 5.4);
        
        addBidirectionalSegment("Mong Kok", "Kowloon Tong", "MTR", 6, 5.7);
        addBidirectionalSegment("Kowloon Tong", "Hung Hom", "MTR", 7, 6.5);
        addBidirectionalSegment("Kowloon Tong", "Wong Tai Sin", "MTR", 5, 4.8);
        addBidirectionalSegment("Wong Tai Sin", "Kwun Tong", "MTR", 10, 7.1);
        

        addBidirectionalSegment("Mong Kok", "Kwun Tong", "Red_Minibus", 25, 12.0);
        addBidirectionalSegment("Tsim Sha Tsui", "Hung Hom", "Walk", 15, 0.0);

        // --- Cross-Harbor Routes ---

        addBidirectionalSegment("Tsim Sha Tsui", "Central", "Ferry", 10, 4.0); // 假设天星小轮到中环
        addBidirectionalSegment("Tsim Sha Tsui", "Admiralty", "MTR", 5, 10.5); 
        addBidirectionalSegment("Hung Hom", "Admiralty", "MTR", 5, 10.5); // 东铁线过海
        
        
        addBidirectionalSegment("Hung Hom", "HKU", "Cross_Bus", 40, 16.0);


        addBidirectionalSegment("Central", "Admiralty", "MTR", 2, 4.8);
        addBidirectionalSegment("Central", "Admiralty", "Walk", 15, 0.0);
        
        addBidirectionalSegment("Central", "HKU", "MTR", 7, 5.7);
        addBidirectionalSegment("Central", "HKU", "Bus", 15, 6.2);
        
        addBidirectionalSegment("Admiralty", "HKU", "MTR", 8, 5.7);
    }
};