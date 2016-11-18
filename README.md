# OS_FAT12
A simple shell that runs a few commands to navigate a fat12 system, written in C.
Commands must be lowercase, arguments may be lowercase or uppercase.
Welcome to the most underwhelming shell you've seen in your life.

Available Commands:

	cat x: shows contents of file x

	cd x: changes directory to path x

	df: shows free disk space

	ls x: lists the entry data for given x

	mkdir x: creates a directory in specified location x

	pbs: prints boot sector information

	pfe x y: prints the fat entries from range x to y

	pwd: prints the current working directory

	rm x: removes a specified file x

	rmdir x: removes a specified directory x

	touch x: creates a file in specified path x

To compile simply run the ./setup.bash file, this will also make fresh copies of the floppies and start the shell. To start the shell without recompiling and refreshing the floppies run ./fat12 .
