#include "shell.h"

/*****************************************************************************
 * executeCmd
 *
 * Get the user input from the shell
 *
 * Return: the user input
 ****************************************************************************/
void executeCmd(char **args) {
	pid_t  pid;
	int    status;

	char command[] = "./commands/";


	if ((pid = fork()) < 0) {     // fork a child
		printf("Error forking child process\n");
		exit(1);
	} 
	else if (pid == 0) {          //do stuff in child

		//use switch statement for different commands??
		if (execvp( strcat(command,args[0]), args ) < 0) {    
			printf("Error executing command\n");
			exit(1);
		}

	} 
	else {                                  
	  while (wait(&status) != pid);       //wait until child process ends
	}
}


/*****************************************************************************
 * getInput
 *
 * Get the user input from the shell
 *
 * Return: the user input
 ****************************************************************************/
char* getInput(CurrentPath cwd){

	char* userInput = NULL;
	ssize_t inputSize = 0;
	
	printf("%s>", cwd.path); //add path before > later
	getline(&userInput, &inputSize, stdin);

	return userInput;

}

/*****************************************************************************
 * parseInput
 *
 * Parse user input into command and arguments
 *
 * input: user input to be parsed
 *
 * Return: a custom argv alternative with commands and args
 ****************************************************************************/
char** parseInput(char* input){

	int location = 0, 
		bufSize = DEFAULT_BUF_SIZE,
		bufLoc = 0;

	const char quoteDelim[2] = "\'\"";
	const char spaceDelim[2] = " \n";
	char* token;
	char* holder = (char*)malloc(DEFAULT_BUF_SIZE* sizeof(char));
	char buffer[DEFAULT_BUF_SIZE]= "";

	holder = input;

	char** parsedInput = (char**)malloc(DEFAULT_BUF_SIZE* sizeof(char));

	//strip single/double quotes
	if( strstr(input, quoteDelim) != NULL ) {

		holder = buffer; //why? idk

		token = strtok(input, quoteDelim);
		if( token == NULL )
			printf("Error quote delimiting\n");


		// loop through until all tokens are handed off to parsedInput
		while(token != NULL){

			//wait to use strcat bc need to check for buffer realloc first
			bufLoc += strlen(token);

			/* if the buffer is too small, reallocate space in 50byte increments */
			if(bufLoc >= bufSize){

				bufSize += DEFAULT_BUF_SIZE;
				holder = (char*)realloc(holder, bufSize * sizeof(char*));
				//add error handling for reallocation of holder

			}

			//concatenate string to be space delimited below
			strcat(holder, token);

			token = strtok(NULL, quoteDelim);

		}
	} 

	// space delimited tokenizing 
	token = strtok(holder, spaceDelim);
	if( token == NULL )
		printf("Error space delimiting\n");

	// loop through until all tokens are handed off to parsedInput
	while(token!= NULL){

		parsedInput[location] = token;
		location++;

		// if the buffer is too small, reallocate space in 50byte increments
		if(location >= bufSize){

			bufSize += DEFAULT_BUF_SIZE;
			parsedInput = (char**)realloc(parsedInput, bufSize * sizeof(char*));

			//maybe add error handling for reallocation of parsedInput
		}

		token = strtok(NULL, spaceDelim);
	}

	parsedInput[location] = NULL; //set null terminating char for args
		
	return parsedInput;

}





