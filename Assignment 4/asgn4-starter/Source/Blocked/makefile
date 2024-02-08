#Make environment
CC=gcc
CFLAGS=-I. -c -g -Wall
LINKARGS=-lm
LIBS=-lm

#Files
SOURCE_FILES= blockedmat.c
OBJECT_FILES= blockedmat.o

#Productions
all : blockedmat

blockedmat : $(OBJECT_FILES)
		$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@ $(LIBS)
		
clean :
		rm -f blockedmat $ $(OBJECT_FILES)
