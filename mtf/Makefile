# 
CC=g++
# 
FLAGS=-Wall -O3 -std=c++11


FILES=main motif graph
SOURCES=$(FILES:=.c)
OBJECTS=$(FILES:=.o)
EXECUTABLES=finder

all: $(SOURCES) $(EXECUTABLES)
	
$(EXECUTABLES): $(OBJECTS) 
	$(CC) $(OBJECTS) $(FLAGS) -o $(EXECUTABLES)

$(FILES):
	$(CC) $(FLAGS) $@.c -o $@.o

clean:
	rm -rf *o finder
