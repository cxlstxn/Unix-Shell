#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  /* DO WHILE SHELL HAS NOT TERMINATED: */
  int x = 1; 
  while (x == 1) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd)); // gets working directory


    /* DISPLAY PROMPT: */
    printf("%s>$", cwd); // command line 
    char userinput[100];


    /* READ AND PARSE USER INPUT: */
    fgets(userinput, sizeof(userinput), stdin); // getting user input

    char* tokens[100];
    int token_count = 0;
    char* token = strtok(userinput, " < \t | > & ;"); // tokens to look for
    while (token != NULL) {
      char *newline = strchr(token, '\n');
      if (newline) {
	*newline = '\0'; // setting NULL position
      }
      tokens[token_count++] = token;
      token = strtok(NULL, " < \t | > & ;");
    }
    tokens[token_count++] = NULL; //last has to be null for execvp to work


    /* IF COMMAND IS BUILT-IN INVOKE APPROPRIATE FUNCTIONS: */

    // Ctrl+d check
    if (feof(stdin)) { // ctrl+d -> exit program
      printf("\n");
      break;
    }
	
    // exit check 
    if (strcmp(userinput, "exit\n") == 0) { //exit -> exit program 
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
      execvp(tokens[0],tokens); // passes user input[0] (function) and rest of string as arguement
      fprintf(stderr, "Error executing command!\n"); // shouldn't ever reach this part - reach it when command invlaid - WHY?
      return 1;
    }
    else{
      wait(NULL); // Parent waiting for child process to complete
   }
    
  }



  
}
