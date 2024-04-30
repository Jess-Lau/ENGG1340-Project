# Compiler
CXX := g++
# Compiler flags
CXXFLAGS := -pedantic-errors -std=c++11
# Ncurse flags
NCFLAGS := $$(ncursesw5-config --cflags --libs) -lncurses -DNCURSES_STACTIC
# Executable name
TARGET := dungeon

# Source files
SRCS := $(wildcard *.cpp)
# Header files
INCS := $(wildcard *.h)
# Object files
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

# Rule to build the executable
$(TARGET): $(OBJS) $(INCS)
	$(CXX) $(CXXFLAGS) $< -o $@ $(NCFLAGS)
	mkdir -p save

# Rule to build object files
$(OBJDIR)%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(NCFLAGS)

# Clean rule
clean:
	rm -rf $(OBJDIR)*.o $(TARGET) save map.txt

.PHONY: clean