#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <vector>

#include "TransportNetwork.h"
#include "Algorithm.h"

namespace {
struct ScenarioDefinition {
    std::string title;
    std::string goal;
    std::string networkLayout;
    std::string origin;
    std::string destination;
    int preferenceMode;
    bool useBudgetLimit;
    double budgetLimit;
};

void printDivider() {
    std::cout << "============================================================\n";
}

void printRegionSeparator() {
    std::cout << "=======================\n";
}

std::string trim(const std::string& text) {
    std::size_t start = text.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    std::size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return trim(value);
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

double readDouble(const std::string& prompt) {
    double value = 0.0;
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

std::string stationRegion(const std::string& station) {
    if (station == "Sham Shui Po" || station == "Mong Kok" ||
        station == "Tsim Sha Tsui" || station == "Kowloon Tong" ||
        station == "Hung Hom" || station == "Wong Tai Sin" ||
        station == "Kwun Tong") {
        return "Kowloon";
    }

    if (station == "Central" || station == "Admiralty" || station == "HKU") {
        return "Hong Kong Island";
    }

    if (station == "Sha Tin") {
        return "New Territories";
    }

    if (station == "Airport") {
        return "Airport Area";
    }

    return "Other Areas";
}

void printStopsForRegion(const std::vector<std::string>& stations, const std::string& region,
                         std::size_t& stopNumber) {
    std::cout << region << '\n';
    bool hasStops = false;
    for (const std::string& station : stations) {
        if (stationRegion(station) == region) {
            std::cout << stopNumber << ". [" << region << "] " << station << '\n';
            ++stopNumber;
            hasStops = true;
        }
    }

    if (!hasStops) {
        std::cout << "No stops in this region.\n";
    }
}

bool hasStopsInRegion(const std::vector<std::string>& stations, const std::string& region) {
    for (const std::string& station : stations) {
        if (stationRegion(station) == region) {
            return true;
        }
    }
    return false;
}

int countStopsInRegion(const std::vector<std::string>& stations, const std::string& region) {
    int count = 0;
    for (const std::string& station : stations) {
        if (stationRegion(station) == region) {
            ++count;
        }
    }
    return count;
}

std::vector<std::string> collectTravelModes(const TransportNetwork& network) {
    std::set<std::string> modeSet;
    for (const std::string& station : network.getStations()) {
        const std::vector<Segment>& neighbors = network.getNeighbors(station);
        for (const Segment& segment : neighbors) {
            modeSet.insert(segment.mode);
        }
    }
    return std::vector<std::string>(modeSet.begin(), modeSet.end());
}

void printCommaSeparated(const std::vector<std::string>& values) {
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << values[i];
    }
    std::cout << '\n';
}

void printWelcome(const TransportNetwork& network) {
    printDivider();
    std::cout << "           Hong Kong Transport Route Planner\n";
    printDivider();
    std::cout << "Stops loaded: " << network.getStopCount() << '\n';
    std::cout << "Segments loaded: " << network.getSegmentCount() << '\n';
    printDivider();
}

void printMainMenu() {
    std::cout << "1. List all stops\n";
    std::cout << "2. Query journeys\n";
    std::cout << "3. Show network summary\n";
    std::cout << "4. Load sample network\n";
    std::cout << "5. Load network from custom file\n";
    std::cout << "6. Scenario / Case Study Mode\n";
    std::cout << "7. Exit\n";
}

void listStops(const TransportNetwork& network) {
    printDivider();
    std::cout << "All Stops\n";
    printDivider();
    std::vector<std::string> stations = network.getStations();
    std::size_t stopNumber = 1;
    const std::vector<std::string> regions = {
        "Kowloon",
        "Hong Kong Island",
        "New Territories",
        "Airport Area",
        "Other Areas"
    };

    bool printedRegion = false;
    for (const std::string& region : regions) {
        if (!hasStopsInRegion(stations, region)) {
            continue;
        }
        if (printedRegion) {
            printRegionSeparator();
        }
        printStopsForRegion(stations, region, stopNumber);
        printedRegion = true;
    }

    printDivider();
}

void showNetworkSummary(const TransportNetwork& network) {
    printDivider();
    std::cout << "Network Summary\n";
    printDivider();
    std::cout << "Total stops: " << network.getStopCount() << '\n';
    std::cout << "Total segments: " << network.getSegmentCount() << '\n';
    std::vector<std::string> modes = collectTravelModes(network);
    std::cout << "Travel modes: ";
    printCommaSeparated(modes);

    std::vector<std::string> stations = network.getStations();
    std::cout << "Region breakdown:\n";
    const std::vector<std::string> regions = {
        "Kowloon",
        "Hong Kong Island",
        "New Territories",
        "Airport Area",
        "Other Areas"
    };
    for (const std::string& region : regions) {
        int count = countStopsInRegion(stations, region);
        if (count > 0) {
            std::cout << "  " << region << ": " << count << " stops\n";
        }
    }
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

bool validateJourneyInput(const TransportNetwork& network, const std::string& startStation,
                          const std::string& endStation) {
    if (!network.hasStation(startStation) || !network.hasStation(endStation)) {
        std::cout << "Unknown station name. Please choose from the loaded network.\n";
        printDivider();
        return false;
    }

    if (startStation == endStation) {
        std::cout << "Start and destination cannot be the same.\n";
        printDivider();
        return false;
    }

    return true;
}

std::vector<PathResult> collectRankedJourneys(const TransportNetwork& network,
                                              const std::string& startStation,
                                              const std::string& endStation, int mode,
                                              bool useBudgetLimit = false,
                                              double budgetLimit = 0.0) {
    std::vector<PathResult> journeys =
        generateCandidateJourneys(network, startStation, endStation, 7, 1000);

    if (useBudgetLimit) {
        journeys.erase(std::remove_if(journeys.begin(), journeys.end(),
                                      [budgetLimit](const PathResult& result) {
                                          return result.totalCost > budgetLimit;
                                      }),
                       journeys.end());
    }

    rankJourneys(journeys, mode);
    return journeys;
}

void printTopJourneys(const std::vector<PathResult>& journeys, int limit = 3) {
    int topCount = static_cast<int>(journeys.size());
    if (topCount > limit) {
        topCount = limit;
    }

    for (int i = 0; i < topCount; ++i) {
        printJourney(journeys[i], i + 1);
    }
}

void printScenarioSummary(const ScenarioDefinition& scenario) {
    printDivider();
    std::cout << "Scenario: " << scenario.title << '\n';
    std::cout << "Goal: " << scenario.goal << '\n';
    std::cout << "Network layout: " << scenario.networkLayout << '\n';
    std::cout << "Origin: " << scenario.origin << '\n';
    std::cout << "Destination: " << scenario.destination << '\n';
    std::cout << "Preference: " << preferenceName(scenario.preferenceMode) << '\n';
    if (scenario.useBudgetLimit) {
        std::cout << "Budget limit: $" << std::fixed << std::setprecision(1)
                  << scenario.budgetLimit << '\n';
    }
    printDivider();
}

void queryJourneys(const TransportNetwork& network) {
    printDivider();
    std::cout << "Journey Query\n";
    printDivider();

    std::string startStation = readLine("Enter start station: ");
    std::string endStation = readLine("Enter destination: ");

    if (!validateJourneyInput(network, startStation, endStation)) {
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
        collectRankedJourneys(network, startStation, endStation, mode);

    if (journeys.empty()) {
        std::cout << "No candidate journeys found with the current search limit.\n";
        printDivider();
        return;
    }

    rankJourneys(journeys, mode);

    std::cout << "Preference: " << preferenceName(mode) << '\n';
    std::cout << "Top ranked candidate journeys:\n";
    std::cout << "------------------------------------------------------------\n";

    printTopJourneys(journeys);

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

    std::string message;
    if (network.loadFromFile(filename, &message)) {
        std::cout << "Network loaded successfully from: " << filename << '\n';
        std::cout << "Stops loaded: " << network.getStopCount() << '\n';
        std::cout << "Segments loaded: " << network.getSegmentCount() << '\n';
        if (!message.empty()) {
            std::cout << message << '\n';
        }
    } else {
        std::cout << "Failed to load network file.\n";
        if (!message.empty()) {
            std::cout << message << '\n';
        }
        std::cout << "Expected format: stop1,stop2,mode,duration,cost\n";
    }
    printDivider();
}

void printScenarioMenu() {
    std::cout << "1. Budget commuter\n";
    std::cout << "2. Last-minute student\n";
    std::cout << "3. Transfer-averse user\n";
    std::cout << "4. Budget-capped office commuter\n";
    std::cout << "5. Custom budget query\n";
    std::cout << "6. Return to main menu\n";
}

void runPredefinedScenario(const TransportNetwork& network, const ScenarioDefinition& scenario) {
    printScenarioSummary(scenario);

    if (!validateJourneyInput(network, scenario.origin, scenario.destination)) {
        return;
    }

    std::vector<PathResult> journeys = collectRankedJourneys(
        network, scenario.origin, scenario.destination, scenario.preferenceMode,
        scenario.useBudgetLimit, scenario.budgetLimit);

    if (journeys.empty()) {
        std::cout << "No candidate journeys matched this scenario.\n";
        if (scenario.useBudgetLimit) {
            std::cout << "Try increasing the budget limit or adjusting the route.\n";
        }
        printDivider();
        return;
    }

    std::cout << "Scenario results:\n";
    std::cout << "------------------------------------------------------------\n";
    printTopJourneys(journeys);
    printDivider();
}

void runCustomBudgetQuery(const TransportNetwork& network) {
    printDivider();
    std::cout << "Custom Budget Query\n";
    printDivider();

    std::string startStation = readLine("Enter start station: ");
    std::string endStation = readLine("Enter destination: ");

    if (!validateJourneyInput(network, startStation, endStation)) {
        return;
    }

    std::cout << "Choose ranking mode under your budget:\n";
    std::cout << "1. Fastest within budget\n";
    std::cout << "2. Cheapest within budget\n";
    std::cout << "3. Fewest segments within budget\n";
    int mode = readInt("Your choice: ");
    if (mode < 1 || mode > 3) {
        std::cout << "Invalid preference mode.\n";
        printDivider();
        return;
    }

    double budgetLimit = readDouble("Enter maximum budget: $");
    if (budgetLimit < 0.0) {
        std::cout << "Budget cannot be negative.\n";
        printDivider();
        return;
    }

    std::vector<PathResult> journeys =
        collectRankedJourneys(network, startStation, endStation, mode, true, budgetLimit);

    if (journeys.empty()) {
        std::cout << "No candidate journeys found within the budget limit of $"
                  << std::fixed << std::setprecision(1) << budgetLimit << ".\n";
        printDivider();
        return;
    }

    std::cout << "Best journeys within budget $" << std::fixed << std::setprecision(1)
              << budgetLimit << '\n';
    std::cout << "Preference: " << preferenceName(mode) << '\n';
    std::cout << "------------------------------------------------------------\n";
    printTopJourneys(journeys);
    printDivider();
}

void runScenarioMode(const TransportNetwork& network) {
    const std::vector<ScenarioDefinition> scenarios = {
        {"Budget commuter",
         "Find the fastest journey without spending more than a modest budget.",
         "Built-in Hong Kong sample transport network",
         "Sham Shui Po",
         "HKU",
         1,
         true,
         25.0},
        {"Last-minute student",
         "Reach campus as quickly as possible, even if the route costs more.",
         "Built-in Hong Kong sample transport network",
         "Sham Shui Po",
         "HKU",
         1,
         false,
         0.0},
        {"Transfer-averse user",
         "Prefer routes with fewer segments to reduce transfers and confusion.",
         "Built-in Hong Kong sample transport network",
         "Sham Shui Po",
         "HKU",
         3,
         false,
         0.0},
        {"Budget-capped office commuter",
         "Travel to the office as quickly as possible while staying below a fixed budget.",
         "Built-in Hong Kong sample transport network",
         "Kwun Tong",
         "Central",
         1,
         true,
         30.0}
    };

    bool running = true;
    while (running) {
        printDivider();
        std::cout << "Scenario / Case Study Mode\n";
        printDivider();
        printScenarioMenu();
        int choice = readInt("Choose a scenario: ");

        if (choice >= 1 && choice <= 4) {
            runPredefinedScenario(network, scenarios[choice - 1]);
        } else if (choice == 5) {
            runCustomBudgetQuery(network);
        } else if (choice == 6) {
            running = false;
        } else {
            std::cout << "Invalid scenario choice.\n";
            printDivider();
        }
    }
}

void loadDefaultNetwork(TransportNetwork& network) {
    network.buildMap();
    std::cout << "Sample network loaded successfully.\n";
    std::cout << "Stops loaded: " << network.getStopCount() << '\n';
    std::cout << "Segments loaded: " << network.getSegmentCount() << '\n';
    printDivider();
}
}

int main() {
    TransportNetwork network;
    network.buildMap();

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
            loadDefaultNetwork(network);
        } else if (choice == 5) {
            loadNetwork(network);
        } else if (choice == 6) {
            runScenarioMode(network);
        } else if (choice == 7) {
            running = false;
            std::cout << "Thank you for using the Transport App. Goodbye!\n";
        } else {
            std::cout << "Invalid menu choice.\n";
            printDivider();
        }
    }

    return 0;
}
