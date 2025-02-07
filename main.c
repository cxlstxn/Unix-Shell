#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  /* SAVE THE CURRENT PATH */
  char* originalEnvPath = getevn();
  
  /* DO WHILE SHELL HAS NOT TERMINATED: */
  int x = 1; 
  while (x == 1) {
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

  // Ctrl+d check
    if (feof(stdin)) { // ctrl+d -> exit program
      printf("\n");
      break;
    }
    
    // exit check 
    if (tokenList[0] != NULL && (strcmp(tokenList[0], "exit") == 0)) { //exit -> exit program
      // tokenList[0] contains the first argument - in this case 'exit' - check if that is not NULL and is 'exit'
      break;
    }
    
    /* ELSE EXECUTE COMMAND AS AN EXTERNAL PROCESS: */

    pid_t pid;
    pid = fork();

    if(pid<0){ // negative pid -> failure
      fprintf(stderr, "Fork failed\n"); // Fork failed
      return 1;
    }
    else if(pid==0){ // signifies child process
      execvp(tokenList[0],tokenList); // passes user input[0] (function) and rest of string as arguement
      /*
	Only reach this section if the command cannot be located, tokenList[0] == NULL, or cannot execute program - not our fault
       */
      fprintf(stderr, "Command not found!\n"); 
      return 1;
    }
    else{
      wait(NULL); // Parent waiting for child process to complete
   }
    
  }



  
}
