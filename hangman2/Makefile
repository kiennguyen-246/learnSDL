OBJS = main.cpp game.cpp init.cpp keyboard.cpp hintBox.cpp livesBox.cpp

CC = g++ -std=c++17

INCLUDE_PATHS = -IC:\mingw32\include\SDL2

LIBRARY_PATHS = -LC:\mingw32\lib

COMPILER_FLAG = -w

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

OBJ_NAME = hangman

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAG) $(LINKER_FLAGS) -o $(OBJ_NAME)