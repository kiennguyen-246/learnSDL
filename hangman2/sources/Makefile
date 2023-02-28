OBJS1 = main.cpp game.cpp init.cpp keyboard.cpp hintBox.cpp livesBox.cpp 
OBJS2 = guessWord.cpp dictionary.cpp hangmanModel.cpp playgamePage.cpp startPage.cpp chooseDifficultyPage.cpp

CC = g++ -std=c++17

INCLUDE_PATHS = -ISDL2\include\SDL2

LIBRARY_PATHS = -LSDL2\lib

COMPILER_FLAG = -w -Wl,-subsystem,windows

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

OBJ_NAME = ..\hangman.exe

all : $(OBJS)
	$(CC) $(OBJS1) $(OBJS2) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAG) $(LINKER_FLAGS) -o $(OBJ_NAME)
	
	