command: shmem.o cmdSupport.o pbs.o pfe.o ls.o cd.o pwd.o rm.o cat.o mkdir.o df.o touch.o rmdir.o
	gcc -o pbs pbs.o cmdSupport.o
	gcc -o pfe pfe.o cmdSupport.o 
	gcc -o ls ls.o cmdSupport.o shmem.o
	gcc -o cd cd.o cmdSupport.o shmem.o
	gcc -o pwd pwd.o cmdSupport.o shmem.o
	gcc -o rm rm.o cmdSupport.o shmem.o
	gcc -o cat cat.o cmdSupport.o shmem.o
	gcc -o mkdir mkdir.o cmdSupport.o shmem.o
	gcc -o df df.o cmdSupport.o
	gcc -o touch touch.o cmdSupport.o shmem.o
	gcc -o rmdir rmdir.o cmdSupport.o shmem.o

shmem.o: ../utilities/shmem.c ../utilities/shmem.h
	gcc -c ../utilities/shmem.c

cmdSupport.o: cmdSupport.c cmdSupport.h
	gcc -c cmdSupport.c


ls: ls.c 
	gcc -g -o ls ls.c cmdSupport.c ../utilities/shmem.c -I.

pbs.o: pbs.c cmdSupport.h 
	gcc -c pbs.c cmdSupport.c

pfe.o: pfe.c cmdSupport.h 
	gcc -c pfe.c cmdSupport.c 

ls.o: ls.c cmdSupport.h ../utilities/shmem.h
	gcc -c ls.c cmdSupport.c ../utilities/shmem.c

cd.o: cd.c cmdSupport.h ../utilities/shmem.h
	gcc -c cd.c cmdSupport.c ../utilities/shmem.c

pwd.o: pwd.c cmdSupport.h ../utilities/shmem.h
	gcc -c pwd.c cmdSupport.c ../utilities/shmem.c

rm.o: rm.c cmdSupport.h ../utilities/shmem.h
	gcc -c rm.c cmdSupport.c ../utilities/shmem.c

cat.o: cat.c cmdSupport.h ../utilities/shmem.h
	gcc -c cat.c cmdSupport.c ../utilities/shmem.c

mkdir.o: mkdir.c cmdSupport.h ../utilities/shmem.h
	gcc -c mkdir.c cmdSupport.c ../utilities/shmem.c

df.o: df.c cmdSupport.h
	gcc -c df.c cmdSupport.c

touch.o: touch.c cmdSupport.h ../utilities/shmem.h
	gcc -c touch.c cmdSupport.c ../utilities/shmem.c

rmdir.o: rmdir.c cmdSupport.h ../utilities/shmem.h
	gcc -c rmdir.c cmdSupport.c ../utilities/shmem.c
