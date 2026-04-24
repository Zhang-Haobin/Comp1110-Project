CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

TARGET = main

OBJS = main.o TransportNetwork.o dijkstra.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

main.o: main.cpp TransportNetwork.h dijkstra.h
	$(CXX) $(CXXFLAGS) -c main.cpp

TransportNetwork.o: TransportNetwork.cpp TransportNetwork.h
	$(CXX) $(CXXFLAGS) -c TransportNetwork.cpp

dijkstra.o: dijkstra.cpp dijkstra.h
	$(CXX) $(CXXFLAGS) -c dijkstra.cpp

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean