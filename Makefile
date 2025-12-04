CXX = g++
CXXFLAGS = -Wall -g
TARGET = marbles

all: $(TARGET)

$(TARGET): marbles.cpp
	$(CXX) $(CXXFLAGS) marbles.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
