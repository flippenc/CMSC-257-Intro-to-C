# Make environment
CC=gcc
CFLAGS=-I. -c -g -Wall
LINKARGS=-lm
LIBS=-lm

# Files
SOURCE_FILES=	asgn2.c \
				a2support.c
OBJECT_FILES=asgn2.o \
				a2support.o

# Productions
all : asgn2

asgn2 : $(OBJECT_FILES)
	$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@ $(LIBS)

clean : 
	rm -f cmsc257-s16-assign2 $(OBJECT_FILES)
