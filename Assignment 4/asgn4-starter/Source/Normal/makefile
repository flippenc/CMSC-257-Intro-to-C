#Make environment
CC=gcc
CFLAGS=-I. -c -g -Wall
LINKARGS=-lm
LIBS=-lm

#Files
SOURCE_FILES= normalmat.c
OBJECT_FILES= normalmat.o

#Productions
all : normalmat

normalmat : $(OBJECT_FILES)
	$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@ $(LIBS)

clean :
	rm -f normalmat $(OBJECT_FILES)

