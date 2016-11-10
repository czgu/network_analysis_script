# 
CC=g++
# 
FLAGS=-Wall -std=c++11


FILES=main motif graph
SOURCES=$(FILES:=.cc)
OBJECTS=$(FILES:=.o)
EXECUTABLES=finder

all: $(SOURCES) $(EXECUTABLES)

%.o:%.cc
	$(CC) $(FLAGS) -c $< -o $@

$(EXECUTABLES): $(OBJECTS)
	$(CC) $(OBJECTS) $(FLAGS) -o $(EXECUTABLES)

clean:
	rm -rf *o finder
