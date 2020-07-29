#Original framework from
#https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

IDIR =../include
CC=g++
CFLAGS= -lSDL2 -ggdb3 -std=c++11 -Wall -lSDL2_ttf -lSDL2_image -I $(IDIR)

SDIR=src
BDIR=bin
LDIR=$(SDIR)/lib
ODIR=$(SDIR)/obj
IDIR=$(SDIR)/inc 

SRC_FILES = $(wildcard $(SDIR)/*.cpp)
OBJ =	$(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRC_FILES) ) 

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: $(SDIR)/%.cpp $(wildcard $(IDIR)/*.hpp)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/worley: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o core $(BDIR)/*
