Hong Kong Transport Route Planner - User Guide
=============================================

This project is a C++ command-line transport route planner. It uses a small
Hong Kong sample transport network and lets the user compare journeys by time,
cost, number of route segments, and simple scenario settings.


1. Project Files
----------------

main.cpp
    Contains the command-line menu, user input, route display, and scenario
    mode.

TransportNetwork.h / TransportNetwork.cpp
    Store the transport network as an adjacency list. They also provide the
    built-in sample map and the file-loading function.

dijkstra.h / dijkstra.cpp
    Contain route-search logic, candidate journey generation, route ranking,
    and Dijkstra's algorithm for fastest or cheapest reference paths.

sample_network.txt
    A text-file version of the sample transport network. It can be loaded from
    the custom file menu.

Makefile
    Used to compile the program.


2. How to Compile
-----------------

Open a terminal in this folder:

    Comp1110-Project/Comp1110-Project

Then run:

    make

This creates an executable file called:

    main

If you want to remove compiled files, run:

    make clean


3. How to Run
-------------

After compiling, run:

    ./main

When the program starts, it automatically loads the built-in sample network
from TransportNetwork::buildMap().

The program will show this main menu:

    1. List all stops
    2. Query journeys
    3. Show network summary
    4. Load sample network
    5. Load network from custom file
    6. Scenario / Case Study Mode
    7. Exit

Type the menu number and press Enter.


4. Menu Guide
-------------

Option 1: List all stops
    Displays all stops in the current network, grouped by region:
    Kowloon, Hong Kong Island, New Territories, Airport Area, and Other Areas.

Option 2: Query journeys
    Lets you enter a start station, destination, and route preference.

    Preference modes:
        1. Fastest
        2. Cheapest
        3. Fewest segments

    The program prints the top ranked candidate journeys. For Fastest and
    Cheapest queries, it also prints a Dijkstra Reference Result.

    Example input:
        Enter start station: Sham Shui Po
        Enter destination: HKU
        Your choice: 1

Option 3: Show network summary
    Displays:
        total number of stops,
        total number of logical route segments,
        transport modes,
        number of stops in each region.

    In the built-in sample network, the program reports:
        Stops loaded: 12
        Segments loaded: 23

Option 4: Load sample network
    Reloads the built-in sample network from buildMap(). Use this if you loaded
    a custom file and want to return to the default network.

Option 5: Load network from custom file
    Loads transport data from a text file.

    The expected file format is:

        stop1,stop2,mode,duration,cost

    Example line:

        Sham Shui Po,Mong Kok,MTR,4,4.8

    The program treats each line as a bidirectional route segment. This means
    the route can be used in both directions.

    To load the included sample file, enter:

        sample_network.txt

    If you run the program from another folder, enter the full or relative path
    to the file instead.

Option 6: Scenario / Case Study Mode
    Runs predefined travel scenarios. These scenarios use fixed origins,
    destinations, preferences, and sometimes budget limits.

    Scenario menu:

        1. Budget commuter
        2. Last-minute student
        3. Transfer-averse user
        4. Budget-capped office commuter
        5. Custom budget query
        6. Return to main menu

    In Scenario / Case Study Mode, the program prints scenario results only.
    It does not print a separate Dijkstra Reference Result.

Option 7: Exit
    Closes the program.


5. Valid Station Names in the Built-In Network
----------------------------------------------

Use station names exactly as written below:

    Sham Shui Po
    Mong Kok
    Tsim Sha Tsui
    Kowloon Tong
    Wong Tai Sin
    Kwun Tong
    Hung Hom
    Central
    Admiralty
    HKU
    Airport
    Sha Tin

The program checks for unknown station names. If the name is not in the loaded
network, it will ask the user to choose from the loaded network.


6. Notes About Route Results
----------------------------

Candidate journeys:
    The program uses DFS to generate candidate journeys with a maximum search
    depth and a maximum number of returned journeys. These journeys are then
    ranked according to the selected preference.

Dijkstra Reference Result:
    In normal Query journeys mode, Fastest and Cheapest queries also show a
    Dijkstra reference result. This is the optimal route for the selected single
    objective: time for Fastest, cost for Cheapest.

Fewest segments:
    Fewest-segment mode ranks generated candidate journeys by segment count.
    There is no separate Dijkstra reference for this mode.

Budget scenarios:
    Scenario mode can filter out journeys that cost more than the budget limit.
    The remaining journeys are ranked according to the scenario preference.


7. Common Problems
------------------

Problem: "Unknown station name."
    Check spelling, capitalization, and spaces. For example, use:
        Sham Shui Po
    not:
        sham shui po

Problem: "Start and destination cannot be the same."
    Choose two different stations.

Problem: "Failed to load network file."
    Check that the file path is correct and that the file follows this format:
        stop1,stop2,mode,duration,cost

Problem: "Invalid preference mode" or "Invalid menu choice."
    Enter one of the numbers shown in the menu.


8. Example Full Run
-------------------

Compile:

    make

Run:

    ./main

Choose journey query:

    2

Enter:

    Enter start station: Sham Shui Po
    Enter destination: HKU
    Your choice: 1

The program will show the fastest ranked candidate journeys and the Dijkstra
reference result.
