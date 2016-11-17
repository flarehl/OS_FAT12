command: pbs pfe ls cd pwd rm cat mkdir

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

rm: rm.c
	gcc -o rm rm.c cmdSupport.c ../utilities/shmem.c -I.

cat: cat.c
	gcc -o cat cat.c cmdSupport.c ../utilities/shmem.c -I.

mkdir: mkdir.c
	gcc -o mkdir mkdir.c cmdSupport.c ../utilities/shmem.c -I.