# Compiler and Linker
CXX := g++

# Compiler and Linker Flags
CFLAGS := -Wall -Wextra -std=c++11 -g
LDFLAGS := -L/opt/homebrew/opt/sdl2/lib -L/opt/homebrew/opt/sdl2_image/lib
INCLUDES := -I/opt/homebrew/opt/sdl2/include/SDL2 -I/opt/homebrew/opt/sdl2_image/include/SDL2 -I../include
LIBS := -lSDL2 -lSDL2_image

# Source files and Object files
SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard ../include/*.hpp)
OBJECTS := $(SOURCES:.cpp=.o)

# Executable name
EXEC := main

# Default target
all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(EXEC) $(LIBS)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXEC)

.PHONY: all clean
