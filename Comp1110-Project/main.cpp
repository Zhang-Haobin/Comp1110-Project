#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "TransportNetwork.h"
#include "dijkstra.h"

namespace {
const std::string kDefaultNetworkFile = "sample_network.txt";

void printDivider() {
    std::cout << "============================================================\n";
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

int readInt(const std::string& prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Invalid number. Please try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string preferenceName(int mode) {
    if (mode == 1) {
        return "Fastest";
    }
    if (mode == 2) {
        return "Cheapest";
    }
    return "Fewest Segments";
}

void printWelcome(const TransportNetwork& network) {
    printDivider();
    std::cout << "           Hong Kong Transport Route Planner\n";
    printDivider();
    std::cout << "Map image: ";
    std::cout << "Stops loaded: " << network.getStopCount() << '\n';
    std::cout << "Segments loaded: " << network.getSegmentCount() << '\n';
    printDivider();
}

void printMainMenu() {
    std::cout << "1. List all stops\n";
    std::cout << "2. Query journeys\n";
    std::cout << "3. Show network summary\n";
    std::cout << "4. Load network from file\n";
    std::cout << "5. Exit\n";
}

void listStops(const TransportNetwork& network) {
    printDivider();
    std::cout << "All Stops\n";
    printDivider();
    std::vector<std::string> stations = network.getStations();
    for (std::size_t i = 0; i < stations.size(); ++i) {
        std::cout << i + 1 << ". " << stations[i] << '\n';
    }
    printDivider();
}

void showNetworkSummary(const TransportNetwork& network) {
    printDivider();
    std::cout << "Network Summary\n";
    printDivider();
    std::cout << "Total stops: " << network.getStopCount() << '\n';
    std::cout << "Total segments: " << network.getSegmentCount() << '\n';
    printDivider();
}

void printJourney(const PathResult& result, int index) {
    std::cout << "Journey " << index << '\n';
    if (!result.found) {
        std::cout << "  No route found.\n";
        return;
    }

    std::cout << "  Stations: ";
    for (std::size_t i = 0; i < result.stations.size(); ++i) {
        if (i > 0) {
            std::cout << " -> ";
        }
        std::cout << result.stations[i];
    }
    std::cout << '\n';

    for (std::size_t i = 0; i < result.steps.size(); ++i) {
        const PathStep& step = result.steps[i];
        std::cout << "  Step " << i + 1 << ": " << step.from << " -> " << step.to
                  << " by " << step.mode
                  << " (" << step.duration << " min, $"
                  << std::fixed << std::setprecision(1) << step.cost << ")\n";
    }

    std::cout << "  Total time: " << result.totalTime << " min\n";
    std::cout << "  Total cost: $" << std::fixed << std::setprecision(1) << result.totalCost << '\n';
    std::cout << "  Segments: " << result.segmentCount << '\n';
    std::cout << "------------------------------------------------------------\n";
}

void queryJourneys(const TransportNetwork& network) {
    printDivider();
    std::cout << "Journey Query\n";
    printDivider();

    std::string startStation = readLine("Enter start station: ");
    std::string endStation = readLine("Enter destination: ");

    if (!network.hasStation(startStation) || !network.hasStation(endStation)) {
        std::cout << "Unknown station name. Please choose from the loaded network.\n";
        printDivider();
        return;
    }

    if (startStation == endStation) {
        std::cout << "Start and destination cannot be the same.\n";
        printDivider();
        return;
    }

    std::cout << "Choose preference mode:\n";
    std::cout << "1. Fastest\n";
    std::cout << "2. Cheapest\n";
    std::cout << "3. Fewest segments\n";
    int mode = readInt("Your choice: ");

    if (mode < 1 || mode > 3) {
        std::cout << "Invalid preference mode.\n";
        printDivider();
        return;
    }

    std::vector<PathResult> journeys =
        generateCandidateJourneys(network, startStation, endStation, 6, 12);

    if (journeys.empty()) {
        std::cout << "No candidate journeys found with the current search limit.\n";
        printDivider();
        return;
    }

    rankJourneys(journeys, mode);

    std::cout << "Preference: " << preferenceName(mode) << '\n';
    std::cout << "Top ranked candidate journeys:\n";
    std::cout << "------------------------------------------------------------\n";

    int topCount = static_cast<int>(journeys.size());
    if (topCount > 3) {
        topCount = 3;
    }

    for (int i = 0; i < topCount; ++i) {
        printJourney(journeys[i], i + 1);
    }

    if (mode == 1 || mode == 2) {
        bool optimizeByTime = (mode == 1);
        PathResult bestPath = dijkstra(network, startStation, endStation, optimizeByTime);
        std::cout << "Dijkstra Reference Result\n";
        std::cout << "------------------------------------------------------------\n";
        printJourney(bestPath, 1);
    }

    printDivider();
}

void loadNetwork(TransportNetwork& network) {
    std::string filename = readLine("Enter network file path: ");
    if (filename.empty()) {
        std::cout << "File path cannot be empty.\n";
        printDivider();
        return;
    }

    if (network.loadFromFile(filename)) {
        std::cout << "Network loaded successfully from: " << filename << '\n';
        std::cout << "Stops loaded: " << network.getStopCount() << '\n';
        std::cout << "Segments loaded: " << network.getSegmentCount() << '\n';
    } else {
        std::cout << "Failed to load network file. Check that the file exists and is not empty.\n";
        std::cout << "Expected format: stop1,stop2,mode,duration,cost\n";
    }
    printDivider();
}
}

int main() {
    TransportNetwork network;
    if (!network.loadFromFile(kDefaultNetworkFile)) {
        network.buildMap();
    }

    printWelcome(network);

    bool running = true;
    while (running) {
        printMainMenu();
        int choice = readInt("Choose an option: ");

        if (choice == 1) {
            listStops(network);
        } else if (choice == 2) {
            queryJourneys(network);
        } else if (choice == 3) {
            showNetworkSummary(network);
        } else if (choice == 4) {
            loadNetwork(network);
        } else if (choice == 5) {
            running = false;
            std::cout << "Thank you for using the Transport App. Goodbye!\n";
        } else {
            std::cout << "Invalid menu choice.\n";
            printDivider();
        }
    }

    return 0;
}
