# Compiler and Linker
CXX := g++

# Compiler and Linker Flags
CFLAGS := -Wall -Wextra -std=c++11 -g
LDFLAGS := -L/opt/homebrew/opt/sdl2/lib -L/opt/homebrew/opt/sdl2_image/lib
INCLUDES := -I/opt/homebrew/opt/sdl2/include/SDL2 -I/opt/homebrew/opt/sdl2_image/include/SDL2 -I../include
LIBS := -lSDL2 -lSDL2_image

# Source files and Object files
SOURCES := $(wildcard *.cpp) $(wildcard ../some_other_directory/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
DEPS := $(OBJECTS:.o=.d) # Dependency files

# Executable name
EXEC := main

# Default target
all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(EXEC) $(LIBS)

# Target for compiling and generating dependency info
%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

# Include the dependencies
-include $(DEPS)

clean:
	rm -f $(OBJECTS) $(EXEC) $(DEPS)

.PHONY: all clean
