# Pre-compiled header
CORE_PCH_FILENAME = src/all.h

OBJS = src/*.cpp

CC = g++

COMPILER_FLAGS = -w -x c++-header $(CORE_PCH_FILENAME)

LINKER_FLAGS = -lSDL2 -lGL -lGLU

OBJ_NAME = 3DCube

# Targets
all : $(OBJS) 
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
