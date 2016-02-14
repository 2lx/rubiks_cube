# Pre-compiled header
MKDIR = mkdir -p

SRCDIR = src

SRCS = $(subst $(SRCDIR)/, , $(wildcard $(SRCDIR)/*.cpp))

OBJDIR = obj

OBJS = $(patsubst %.cpp, obj/%.o, $(SRCS))

CORE_PCH_FILENAME = $(SRCDIR)/all.h

CORE_PCH = $(subst $(SRCDIR)/, $(OBJDIR)/, $(CORE_PCH_FILENAME).gch)

CC = g++

INCLUDES =

CFLAGS = -Wpedantic -std=c++11

LFLAGS =

LIBS = -lSDL2 -lGL -lGLU -lGLEW -lSDL2_image

TARGET = RCube

RM = rm -f

# Targets
all: directories $(TARGET)

directories:
	$(MKDIR) $(OBJDIR)

$(CORE_PCH):
	$(CC) $(CCFLAGS) -x c++-header $(CORE_PCH_FILENAME) -o $(CORE_PCH)

$(TARGET): $(CORE_PCH) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

$(OBJDIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(TARGET) $(OBJS) $(CORE_PCH)
