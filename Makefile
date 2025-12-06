CXX = g++
CXXFLAGS = -Wall -O3 -DNDEBUG 
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = marbles
SRC = marbles.cpp
HEADERS = GameMaps.h

all: $(TARGET)

$(TARGET): marbles.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) marbles.cpp -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
