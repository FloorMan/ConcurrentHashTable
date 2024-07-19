#target: dependencies
#	action


CC = gcc
CFLAGS = -I #USE: $(CFLAGS)

DEPS = chash.h parseCommands.h #threadHandler.h
OBJ = chash.o parseCommands.o #threadHandler.o

# This rule creates the final executable. To run it type: ~$ ./totalBuild
totalBuild: $(OBJ)
	$(CC) -o $@ $^ 

# The -c flag instructs gcc to only compile to an object file (.o) but
# not use the linker

parseCommands.o: parseCommands.c parseCommands.h
	gcc -c parseCommands.c

chash.o: chash.c chash.h
	gcc -c chash.c

#	Uncomment out the below two lines when the threadhandler is implemented
#threadHandler.o: threadHandler.c threadHandler.h
#	gcc -c threadHandler.c



# Removes the unnecessary object files after the main executable is finished building
# To use, type the instruction: ~$ make clean
.PHONY: clean

clean:
	rm *.o totalBuild