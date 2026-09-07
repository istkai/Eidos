CXX := clang++
CXXFLAGS := -std=c++20 -g -O0

BUILD_DIR := build/manual-make
TARGET := $(BUILD_DIR)/eidos
OBJECTS := $(BUILD_DIR)/main.o $(BUILD_DIR)/pixel_count.o $(BUILD_DIR)/extent.o

.PHONY: all
all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(BUILD_DIR)/main.o: src/main.cpp src/pixel_count.hpp src/extent.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/pixel_count.o: src/pixel_count.cpp src/pixel_count.hpp src/extent.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/extent.o: src/extent.cpp src/extent.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@
