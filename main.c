#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char* originalEnvPath = getenv("PATH"); // gets the original path
    chdir(getenv("HOME")); // changes directory to home path

    /* DO WHILE SHELL HAS NOT TERMINATED: */

  while (1) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd)); // gets working directory


    /* DISPLAY PROMPT: */
    printf("%s>$ ", cwd); // command line 
    char userinput[100];


    /* READ AND PARSE USER INPUT: */
    fgets(userinput, sizeof(userinput), stdin); // getting user input

    char* tokenList[100];
    int token_count = 0;
    char* token = strtok(userinput, " < \t | > & ;"); // tokens to look for
    while (token != NULL) {
      char *newline = strchr(token, '\n');
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
      setenv("PATH", originalEnvPath, 1); // reset path
      break;
    } else if (strcmp(tokenList[0], "exit") == 0) { //exit -> exit program 
      // tokenList[0] contains the first argument - in this case 'exit' - check if that is not NULL and is 'exit'
      setenv("PATH", originalEnvPath, 1); // reset path
      break;
    } else if (strcmp(tokenList[0], "getpath") ==  0) { // getpath -> print path
        printf("%s\n", getenv("PATH")); 
    } else if (strcmp(tokenList[0], "setpath") == 0 ) { // setpath -> set path to first args
        setenv("PATH", tokenList[1], 1);
    } else {
        pid_t pid;
        pid = fork();

        if (pid < 0) { // negative pid -> failure
            fprintf(stderr, "Fork failed\n"); // Fork failed
            return 1;
        } else if (pid == 0) { // signifies child process
            execvp(tokenList[0], tokenList); // passes user input[0] (function) and rest of string as argument
            /*
            Only reach this section if the command cannot be located, tokenList[0] == NULL, or cannot execute program - not our fault
            */
            fprintf(stderr, "Command not found!\n");
            return 1;
        } else {
            wait(NULL); // Parent waiting for child process to complete
        }
    }
  }

}
