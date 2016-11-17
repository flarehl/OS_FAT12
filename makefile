all: command fat12.o shell.o shmem.o
	gcc -o fat12 fat12.o shell.o shmem.o

command: 
	cd commands && $(MAKE)

fat12.o: fat12.c shell.h
	gcc -c fat12 fat12.c shell.c 

shell.o: shell.c shell.h
	gcc -c shell shell.c

shmem.o: ./utilities/shmem.c ./utilities/shmem.h
	gcc -c ./utilities/shmem.c