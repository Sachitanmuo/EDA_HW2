CXX  = g++
CXXFLAGS = -std=c++11
TARGET = Lab02
SOURCES = main.cpp Hypergraph.cpp
HEADERS = Hypergraph.h
all: $(TARGET)
$(TARGET) : $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
.PHONY: all clean