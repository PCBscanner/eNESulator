# Define the link target / name of the executable
LinkTarget = ../bin/eNESulator

#Define the libraries that we need
LIBS = -lSDL2 -lSDL2_ttf

#Define flags
CFLAGS = -std=c++17

#object files that we wish to output
objects = 6502.o apu.o bus.o dmc.o filter.o main.o noise.o ppu.o pulse.o triangle.o

rebuildables = $(objects) $(LinkTarget)

#rule to link the object files and libraries and create a single executable file
$(LinkTarget): $(objects)
	@echo "Rule 1..."
	g++ -g -o $(LinkTarget) $(objects) $(LIBS) $(CFLAGS)

#rule to pre-process and compile the source files into object files
%.o: %.cpp
	@echo "Rule 2..."
	g++ -o $@ -c $< $(CFLAGS)

.PHONEY:
clean:
	rm $(rebuildables)

.PHONEY:
clean_objects:
	rm $(objects)
