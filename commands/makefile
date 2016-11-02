commands: pbs pfe ls cd pwd

pbs: pbs.c
	gcc -o pbs pbs.c cmdSupport.c -I.

pfe: pfe.c
	gcc -o pfe pfe.c cmdSupport.c -I.

ls: ls.c 
	gcc -o ls ls.c cmdSupport.c ../utilities/shmem.c -I.

cd: cd.c
	gcc -o cd cd.c cmdSupport.c ../utilities/shmem.c -I.

pwd: pwd.c
	gcc -o pwd pwd.c cmdSupport.c ../utilities/shmem.c -I.