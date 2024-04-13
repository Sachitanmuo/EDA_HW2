CXX  = g++
CXXFLAGS = -std=c++11 -O3
TARGET = Lab02
SOURCES = main.cpp Hypergraph.cpp
HEADERS = Hypergraph.h
all: $(TARGET)
$(TARGET) : $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
.PHONY: all clean