CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
TARGET = dbms
SOURCES = main.cpp interface.cpp data_file.cpp json_file.cpp Array.cpp linear_list.cpp \
	DoubleLinearList.cpp stack.cpp DoubleQueue.cpp RB_tree.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
