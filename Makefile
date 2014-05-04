EXE=io-lua
CC=cc -Wall
LIB_FLAGS=-L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf -llua -ldl -lm 
INCLUDE_FLAGS=-I/usr/local/include -I/usr/local/include/SDL2 -I./code
CCFLAGS= $(INCLUDE_FLAGS) $(LIB_FLAGS)
BUILD_DIR=build

SRC_COMMON=$(shell find ./code/common -name '*.c' -exec basename {} \;) 
SRC_INPUT=$(shell find ./code/input -name '*.c' -exec basename {} \;)
SRC_VIDEO=$(shell find ./code/video -name '*.c' -exec basename {} \;)
SRC_GAME=$(shell find ./code/game -name '*.c' -exec basename {} \;) 
SRC_SYS=$(shell find ./code/sys -name '*.c' -exec basename {} \;)

SRC=$(SRC_COMMON) $(SRC_VIDEO) $(SRC_GAME) $(SRC_SYS)

OBJ_COMMON=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_COMMON))
OBJ_INPUT=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_INPUT))
OBJ_VIDEO=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_VIDEO))
OBJ_GAME=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_GAME))
OBJ_SYS=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_SYS))

OBJ=$(OBJ_COMMON) $(OBJ_INPUT) $(OBJ_VIDEO) $(OBJ_GAME) $(OBJ_SYS)

all: makedirs $(BUILD_DIR)/$(EXE)

$(BUILD_DIR)/obj/%.o : code/common/%.c 
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)
$(BUILD_DIR)/obj/%.o : code/input/%.c
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)
$(BUILD_DIR)/obj/%.o : code/video/%.c 
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)
$(BUILD_DIR)/obj/%.o : code/game/%.c
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)
$(BUILD_DIR)/obj/%.o : code/sys/%.c
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)

$(BUILD_DIR)/$(EXE) : $(OBJ) 
	$(CC) -g -o $@ $^ $(CCFLAGS)

clean:
	rm -rf $(BUILD_DIR) 
	
makedirs:
	if [ ! -d $(BUILD_DIR)/obj ]; then mkdir -p $(BUILD_DIR)/obj; fi
	rm -rf $(BUILD_DIR)/base
	cp -r base $(BUILD_DIR)

