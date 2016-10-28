all: fat.o fatSupport.o pfe.o pbs.o
	gcc -o fat12 fat.o fatSupport.o
	gcc -o pfe pfe.o fatSupport.o
	gcc -o pbs pbs.o fatSupport.o

fat.o: fat.c fatSupport.h
	gcc -c fat.c

fatSupport.o: fatSupport.c fatSupport.h 
	gcc -c fatSupport.c

pfe.o: pfe.c fatSupport.h
	gcc -c pfe.c

pbs.o: pbs.c fatSupport.h
	gcc -c pbs.c
