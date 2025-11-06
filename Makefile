CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.cpp)

NAME := superPalworldBreed

PAL_PATH := data/pals.json
BREED_PATH := data/breed.json

all: $(NAME)
$(NAME): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: $(NAME)
	./$(NAME) $(PAL_PATH) $(BREED_PATH)

clean:
	rm -f $(NAME)
