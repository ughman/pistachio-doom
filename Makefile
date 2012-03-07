FLAGS := -m32 -g
LDFLAGS := $(FLAGS) `sdl-config --libs`
CCFLAGS := $(FLAGS) -w
CXXFLAGS := $(FLAGS) -include src/LangExt.hpp `sdl-config --cflags`

CCSOURCES  := $(shell find src -name "*.c")
CXXSOURCES := $(shell find src -name "*.cpp")
OBJECTS := $(CCSOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)

all: pistachio-doom

clean:
	rm -f $(OBJECTS)

pistachio-doom: $(OBJECTS)
	g++ $(LDFLAGS) -o $@ $^ `sdl-config --libs`

%.o: %.c
	gcc -c $(CCFLAGS) $< -o $@

%.o: %.cpp
	g++ -c $(CXXFLAGS) $< -o $@

.PHONY: all
