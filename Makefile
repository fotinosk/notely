CXX = g++
CXXFLAGS = -std=c++17
TARGET = notes
SRCS = main.cpp utils.cpp

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET)
