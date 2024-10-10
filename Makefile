COMPILER=gcc

BIN_DIR=bin

SRC_DIR=src

SDL_FLAGS=`pkg-config sdl2 --cflags --libs`

make:
	@mkdir -p $(BIN_DIR)
	$(COMPILER) $(SRC_DIR)/main.c -o $(BIN_DIR)/sdl-conway $(SDL_FLAGS)
