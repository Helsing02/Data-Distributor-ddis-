# Compiler
CXX = g++
CXXFLAGS = -I./$(INCLUDE_DIR) -pthread -Wall -Wextra -Wpedantic -std=c++17

# Dirs
INCLUDE_DIR = include
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build
DOCS_DIR = documentation


SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)


# Target file
TARGET = $(BIN_DIR)/ddist

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
	
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	
$(DOCS_DIR):
	mkdir -p $(DOCS_DIR)
	
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

.PHONY: docs

docs: $(DOCS_DIR)
	cd configs && doxygen
	
clean-docs:
	rm -rf $(DOCS_DIR)
	
rebuild: clean all

clean-all: clean clean-docs



