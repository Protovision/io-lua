EXE=io-lua
CC=cc -Wall
OS=$(shell uname)
ifeq ($(OS), Darwin)
INCLUDE_FLAGS = \
	-I./static/macosx \
	-I./code/libs/SDL2 \
	-I./code/libs/SDL2_image \
	-I./code/libs/SDL2_mixer \
	-I./code/libs/SDL2_ttf \
	-I./code/libs/unzip \
	-I./code/libs/zlib \
	-I./code/libs/lua \
	-I./code/common \
	-I./code/audio \
	-I./code/video

LIB_FLAGS= \
	./static/macosx/*.a -ldl -lm -lpthread -liconv \
	-Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox \
	-Wl,-framework,AudioUnit -Wl,-framework,Carbon \
	-Wl,-framework,ForceFeedback -Wl,-framework,Cocoa \
	-Wl,-framework,IOKit 

else ifeq ($(OS), Linux)
INCLUDE_FLAGS = \
	-I/usr/include/SDL2 \
	-I/usr/include/lua5.2 \
	-I./code/common \
	-I./code/audio \
	-I./code/video \
	-I./code/libs/unzip
LIB_FLAGS= \
	-L/usr/lib/x86_64-linux-gnu \
	-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lz -llua5.2 -lm -ldl
endif

BUILD_DIR=build

SRC_COMMON=$(shell find ./code/common -name '*.c' -exec basename {} \;) 
SRC_VIDEO=$(shell find ./code/video -name '*.c' -exec basename {} \;)
SRC_SYS=$(shell find ./code/sys -name '*.c' -exec basename {} \;)
SRC_AUDIO=$(shell find ./code/audio -name '*.c' -exec basename {} \;)
SRC_UNZIP=$(shell find ./code/libs/unzip -name '*.c' -exec basename {} \;)

SRC=$(SRC_COMMON) $(SRC_VIDEO) $(SRC_SYS) $(SRC_AUDIO) $(SRC_UNZIP)

OBJ_COMMON=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_COMMON))
OBJ_VIDEO=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_VIDEO))
OBJ_SYS=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_SYS))
OBJ_AUDIO=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_AUDIO))
OBJ_UNZIP=$(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(SRC_UNZIP))

OBJ=$(OBJ_COMMON) $(OBJ_VIDEO) $(OBJ_SYS) $(OBJ_AUDIO) $(OBJ_UNZIP)

all: makedirs $(BUILD_DIR)/$(EXE)

$(BUILD_DIR)/obj/%.o : code/common/%.c 
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)
$(BUILD_DIR)/obj/%.o : code/video/%.c 
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)
$(BUILD_DIR)/obj/%.o : code/sys/%.c
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)
$(BUILD_DIR)/obj/%.o : code/audio/%.c
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)
$(BUILD_DIR)/obj/%.o : code/libs/unzip/%.c
	$(CC) -g -c -o $@ $< $(INCLUDE_FLAGS)

$(BUILD_DIR)/$(EXE) : $(OBJ) 
	$(CC) $(INCLUDE_FLAGS) -g -o $@ $^ $(LIB_FLAGS)

clean:
	rm -rf $(BUILD_DIR) 

makedirs:
	if [ ! -d $(BUILD_DIR)/obj ]; then mkdir -p $(BUILD_DIR)/obj; fi
	cp base.zip $(BUILD_DIR)
	cp game.zip $(BUILD_DIR)

