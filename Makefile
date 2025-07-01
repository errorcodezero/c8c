SRC_DIR = src
BUILD_DIR = build
EXE = c8c

SRC = main.c
OBJ = $(SRC:%.c=%.o)

CFLAGS += -std=c++23 -Wall -Wextra -pedantic
# CC = gcc

all: debug

release: setup
release: CFLAGS += -O3 -DNDEBUG
release: $(BUILD_DIR)/$(EXE)

debug: setup
debug: CFLAGS += -g
debug: $(BUILD_DIR)/$(EXE)

.PHONY: clean setup

setup:
	mkdir -p build/

clean:
	rm -rf build

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(EXE): $(BUILD_DIR)/$(OBJ)
	$(CC) $(CFLAGS) $< -o $@
