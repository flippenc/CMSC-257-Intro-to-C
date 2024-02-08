#Make environment
CC=gcc
CFLAGS=-I. -c -g -Wall
LINKARGS=-lm
LIBS=-lm -lrt -lpthread

#Files
SOURCE_FILES= blockedmultithreaded.c
OBJECT_FILES= blockedmultithreaded.o

#Productions
all : blockedmultithreaded

blockedmultithreaded : $(OBJECT_FILES)
		$(CC) $(LINKARGS) $(OBJECT_FILES) -g -o $@ $(LIBS)
		
clean :
		rm -f blockedmultithreaded $ $(OBJECT_FILES)
