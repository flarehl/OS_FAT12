all: commands fat12

commands: 
	cd commands && $ (MAKE)

fat12: fat12.c
	gcc -o fat12 fat12.c shell.c ./utilities/shmem.c -I./utilities


