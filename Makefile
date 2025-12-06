CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = marbles
SRC = marbles.cpp

all: $(TARGET)

$(TARGET): marbles.cpp
	$(CXX) $(CXXFLAGS) marbles.cpp -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
