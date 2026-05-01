CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

TARGET = main

OBJS = main.o TransportNetwork.o Algorithm.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

main.o: main.cpp TransportNetwork.h Algorithm.h
	$(CXX) $(CXXFLAGS) -c main.cpp

TransportNetwork.o: TransportNetwork.cpp TransportNetwork.h
	$(CXX) $(CXXFLAGS) -c TransportNetwork.cpp

Algorithm.o: Algorithm.cpp Algorithm.h
	$(CXX) $(CXXFLAGS) -c Algorithm.cpp

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean