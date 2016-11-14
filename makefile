all: command fat12

command: 
	cd commands && $(MAKE)

fat12: fat12.c
	gcc -o fat12 fat12.c shell.c ./utilities/shmem.c -I./utilities


