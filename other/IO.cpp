#include <iostream>
#include <string>
#include "TransportNetwork.h"

using namespace std;


void printMenu() {
    cout << "=========================================" << endl;
    cout << "   Welcome to Hong Kong Transport App!   " << endl;
    cout << "=========================================" << endl;
    cout << "Available Stations:" << endl;
    cout << "- Sham Shui Po    - Mong Kok       - Tsim Sha Tsui" << endl;
    cout << "- Kowloon Tong    - Hung Hom       - Wong Tai Sin" << endl;
    cout << "- Kwun Tong       - Central        - Admiralty" << endl;
    cout << "- HKU" << endl;
    cout << "-----------------------------------------" << endl;
}

int main() {
    TransportNetwork hkMap;
    hkMap.buildMap();

    printMenu();

    bool keepRunning = true;
    while (keepRunning) {
        string startStation, endStation;
        int mode;

        cout << "\nEnter Start Station: ";
        getline(cin, startStation);

        cout << "Enter Destination: ";
        getline(cin, endStation);

        cout << "Choose optimization preference:\n";
        cout << "1. Fastest Route (Shortest Time)\n";
        cout << "2. Cheapest Route (Lowest Cost)\n";
        cout << "Your choice (1 or 2): ";
        cin >> mode;


        cout << "\nCalculating route..." << endl;
        cout << "-----------------------------------------" << endl;
        hkMap.findShortestPath(startStation, endStation, mode);
        cout << "-----------------------------------------" << endl;

        // 4. 询问是否继续
        cout << "\nDo you want to search another route? (y/n): ";
        char choice;
        cin >> choice;
        

        cin.ignore(10000, '\n'); 

        if (choice == 'n' || choice == 'N') {
            keepRunning = false;
            cout << "Thank you for using the Transport App. Goodbye!" << endl;
        }
    }

    return 0;
}