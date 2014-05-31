EXE=io-lua
CC=cc -Wall
OS=$(shell uname)
ifeq ($(OS), Darwin)
LIB_FLAGS=./libs/macosx/a/*.a -ldl -lm -lz -liconv -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,Carbon -Wl,-framework,ForceFeedback -Wl,-framework,Cocoa -Wl,-framework,IOKit 
INCLUDE_FLAGS=-I./libs/macosx/include/SDL2 -I./libs/lua -I./code
else ifeq ($(OS), Linux)
LIB_FLAGS=-L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf -llua -ldl -lm 
INCLUDE_FLAGS=-I/usr/local/include -I/usr/local/include/SDL2 -I./code
endif

CCFLAGS= $(INCLUDE_FLAGS) $(LIB_FLAGS)
BUILD_DIR=build

SRC_COMMON=$(shell find ./code/common -name '*.c' -exec basename {} \;) 
SRC_VIDEO=$(shell find ./code/video -name '*.c' -exec basename {} \;)
SRC_SYS=$(shell find ./code/sys -name '*.c' -exec basename {} \;)
SRC_AUDIO=$(shell find ./code/audio -name '*.c' -exec basename {} \;)

SRC=$(SRC_COMMON) $(SRC_VIDEO) $(SRC_SYS) $(SRC_AUDIO)

OBJ_COMMON=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_COMMON))
OBJ_VIDEO=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_VIDEO))
OBJ_SYS=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_SYS))
OBJ_AUDIO=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_AUDIO))

OBJ=$(OBJ_COMMON) $(OBJ_VIDEO) $(OBJ_SYS) $(OBJ_AUDIO)

all: makedirs $(BUILD_DIR)/$(EXE)

$(BUILD_DIR)/obj/%.o : code/common/%.c 
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)
$(BUILD_DIR)/obj/%.o : code/video/%.c 
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)
$(BUILD_DIR)/obj/%.o : code/sys/%.c
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)
$(BUILD_DIR)/obj/%.o : code/audio/%.c
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)

$(BUILD_DIR)/$(EXE) : $(OBJ) 
	$(CC) -g -o $@ $^ $(CCFLAGS)

clean:
	rm -rf $(BUILD_DIR) 
	
makedirs:
	if [ ! -d $(BUILD_DIR)/obj ]; then mkdir -p $(BUILD_DIR)/obj; fi
	rsync -ur base $(BUILD_DIR)
	rsync -ur game $(BUILD_DIR)

