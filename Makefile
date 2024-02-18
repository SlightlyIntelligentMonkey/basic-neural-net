RELEASE = 1

ifeq ($(RELEASE), 0)
CXXFLAGS = -Og -g -static
FILENAME = debug
else
CXXFLAGS = -O3 -s -static
#CXXFLAGS = -O3 -g -static
FILENAME = release
endif

CXXFLAGS += 

#include directories
#INCDIR =
#lib directories and libs
#LIBDIR =
#LIBS +=

SOURCES = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)

OBJECTS = $(addprefix obj/$(FILENAME)/, $(SOURCES:%.cpp=%.o))

all: bin/$(FILENAME).exe

bin/$(FILENAME).exe: $(OBJECTS)
	@mkdir -p $(@D)
	@g++ $(CXXFLAGS) $^ -o $@ $(LIBDIR) $(LIBS)
	@echo Finished compiling: $@

obj/$(FILENAME)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo Compiling $^
	@g++ $(CXXFLAGS) $(INCDIR) $^ -o $@ -c

clean:
	@rm -rf build obj
