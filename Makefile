# Pre-compiled header
CORE_PCH_FILENAME = src/all.h

OBJS = src/*.cpp

CC = g++

COMPILER_FLAGS = -x c++-header $(CORE_PCH_FILENAME) -Wpedantic -std=c++11

LINKER_FLAGS = -lSDL2 -lGL -lGLU -lGLEW

OBJ_NAME = 3DCube

# Targets
all : $(OBJS) 
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
