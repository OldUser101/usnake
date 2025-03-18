SRC_DIR := src
BIN_DIR := bin

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRC_FILES))
BIN_USNAKE := $(BIN_DIR)/usnake

CXX := g++
CXXFLAGS := $(shell pkg-config --cflags gtk4) -g
LDFLAGS := $(shell pkg-config --libs gtk4)

all: $(BIN_USNAKE) copy-ui copy-png

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(UI_FILES): $(BIN_DIR)
	cp $(UI_FILES)

$(BIN_USNAKE): $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

copy-ui:
	cp $(SRC_DIR)/*.ui $(BIN_DIR)

copy-png:
	cp $(SRC_DIR)/*.png $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)

.PHONY:
	clean