SRC_DIR := src
OBJ_DIR := build
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := -pthread
CXXFLAGS := -std=c++17 -O3
INC := -I ./include

all: mcts_gomoku
debug: CXXFLAGS += -DDEBUG -pg
debug: mcts_gomoku

mcts_gomoku: $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX)  $(CXXFLAGS) $(INC) $(LDFLAGS)  -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf mcts_gomoku
