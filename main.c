#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

  /* SAVE THE CURRENT PATH */
  char* originalEnvPath = getenv("PATH"); // fetches and stores the original path to restore later
  chdir(getenv("HOME")); // changes directory to user's home path - shell running in user's HOME - good starting point

  /* DO WHILE SHELL HAS NOT TERMINATED: */

  while (1) {

    char cwd[1024]; // Creates buffer
    // get working directory and save it to cwd
    if((getcwd(cwd, sizeof(cwd))) == NULL){
      perror("Failure getting current working directory"); // Failure
      return 1;
    }

    /* DISPLAY PROMPT: */
    printf("%s>$ ", cwd); // command line
    fflush(stdout); // prompt appears immediately

    char userinput[100];

    /* READ AND PARSE USER INPUT: */
    fgets(userinput, sizeof(userinput), stdin); // getting user input

    // for printing the command prompt upon empty input
    if(userinput[0] == '\n'){
      continue;
    }

    char* tokenList[100];
    int token_count = 0;
    char* token = strtok(userinput, " < \t | > & ;"); // tokens to look for
    while (token != NULL) {
      char *newline = strchr(token, '\n'); // searching for '\n'
      if (newline) {
	      *newline = '\0'; // setting NULL position
      }
      tokenList[token_count++] = token;
      token = strtok(NULL, " < \t | > & ;");
    }
    tokenList[token_count++] = NULL; //last has to be null for execvp to work


    /* IF COMMAND IS BUILT-IN INVOKE APPROPRIATE FUNCTIONS: */
    
    if (feof(stdin)) { // ctrl+d -> exit program
      printf("\n");
      setenv("PATH", originalEnvPath, 1); // reset path to original
      break;
    }
    else if (strcmp(tokenList[0], "exit") == 0) { //exit -> exit program
      if(tokenList[1] == NULL){
      // tokenList[0] contains the first argument - 'exit'
      setenv("PATH", originalEnvPath, 1); // reset path
      break;
    }else{
	printf("Error: Too many arguments. Usage exit\n");
      }
    } // getpath function:
    else if (strcmp(tokenList[0], "getpath") ==  0) { // getpath -> print path
      if(tokenList[1] != NULL){
	printf("Error: Too many arguments. Usage getpath\n");
      }else{
        printf("%s\n", getenv("PATH"));
      }
    }
    // setpath function: 
    else if (strcmp(tokenList[0], "setpath") == 0){ // setpath -> set path to first args
      if(tokenList[1] == NULL){
	printf("Error: Missing argument. Usage: setpath <directory>\n");
      }
      else if(tokenList[2] != NULL){
	printf("Error: Too many arguments. Usage setpath <directory>\n");
      }
      else{
        setenv("PATH", tokenList[1], 1);
      }
    }
    // cd Command:
    else if (strcmp(tokenList[0], "cd") == 0) {
      if(tokenList[1] == NULL){
      // type 1 - no args -> put user in home directory:
      chdir(getenv("HOME"));
      } else if(strcmp(tokenList[1], "..") == 0){
	// type 2 - '..' args -> go to parent directory:
        chdir("..");
      } else if (access(tokenList[1], F_OK) == 0) {
	// type 3 - 'filepath' args -> go to directory:
        chdir(tokenList[1]);
      } else {
        printf("Error: directory does not exist\n");
      }
    }
    else{
      /* ELSE EXECUTE COMMAND AS AN EXTERNAL PROCESS: */
        pid_t pid;
        pid = fork();

        if (pid < 0) { // negative pid -> failure
            fprintf(stderr, "Fork failed\n");
            return 1;
        } else if (pid == 0) { // signifies child process
            execvp(tokenList[0], tokenList); // passes userinput[0] (function) and rest of string as arg
            // tokenList[0] == NULL, or cannot execute program - Failure
            fprintf(stderr, "Command not found!\n");
            return 1;
        } else {
            wait(NULL); // Parent waiting for child process to complete
        }
    }



  }
}
