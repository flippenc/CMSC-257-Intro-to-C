#Make environment
CC=gcc
CFLAGS=-I. -c -g -Wall
LINKARGS=-lm
LIBS=-lm

#Files
SOURCE_FILES= transposedmat.c
OBJECT_FILES= transposedmat.o

#Productions
all : transposedmat

transposedmat : $(OBJECT_FILES)
	$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@ $(LIBS)

clean :
	rm -f transposedmat $(OBJECT_FILES)
