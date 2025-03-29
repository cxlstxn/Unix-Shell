#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "commands.h"

// TO FIX:
// exit command should exit firt time on exit - not sure if this is working or not
// deal with reading in >512 char inputs - error deals with this?

// DECLARING GLOBAL HISTORY VARS:

char history_array[HISTORY_SIZE][512]; // store up to 20 commands of size 512 each
int history_count = 0; // number of commands entered
int history_next = 0; //stores next available position in array
char* alias_name[10]; // store up to 10 aliases
char* alias_command[10]; // store up to 10 alias names

// command is user inputted string 
void add_to_history(char* command) {
  // Shift all commands up by one
  for (int i = HISTORY_SIZE - 1; i > 0; i--) {
    strncpy(history_array[i], history_array[i - 1], sizeof(history_array[i]) - 1);
    history_array[i][sizeof(history_array[i]) - 1] = '\0'; 
  }

  // Add the new command to the first position
  strncpy(history_array[0], command, sizeof(history_array[0]) - 1);
  history_array[0][sizeof(history_array[0]) - 1] = '\0'; 

  // Update history count
  if (history_count < HISTORY_SIZE) {
    history_count++;
  }
}

// print_history: prints the history
void print_history(){
  // no commands in history:
  if(history_count == 0){
    printf("No commands in history!\n");
    return;
  }
  for(int i = 0; i < history_count; i++){
    //int index = (history_next - 1 - i + HISTORY_SIZE) % HISTORY_SIZE;
    //printf("%d: %s\n", i+1, history_array[index]);
    printf("%d: %s\n", i+1, history_array[i]);
  }
}


// invoke_history: invokes a command from the history
char* invoke_history(char* userinput){

  // if userinput is !! return the last command
  if(strcmp(userinput, "!!") == 0){
    if(history_count == 0){
      printf("Error: No commands in history\n");
      return "\n";
    }
    return history_array[0]; // Most recent command is at index 0
  }

  // if userinput is a history command:
  int command_index = atoi(&userinput[1]); // convert string to int
  if(command_index == 0){
    printf("Error: Invalid history command\n");
    return "\n";
  }

  // if command index is greater than size of history 
  if(command_index > history_count){
    printf("Error: Command not in history\n");
    return "\n";
  }

  // less than zero - get command from end of history
  if(command_index < 0){
    command_index = history_count + command_index + 1;
    if (command_index < 1){
      printf("Error: Command not in history\n");
      return "\n";
    } else {
      return history_array[history_count - command_index];
    }
  }
  // else 
  return history_array[command_index - 1];
}


// getpath function: Prints the current PATH:
void getpath(char* tokenList[]) {
  if(tokenList[1] != NULL){
    // whitepsace after the function (tokenList[0]) should be treated as valid - currently isn't
    printf("Error: Too many arguments. Usage getpath\n"); // fires when more than one arg (one arg being getpath)
  }else{
    printf("%s\n", getenv("PATH"));
  }
}


// setpath function: Sets new PATH:
void setpath(char* tokenList[]) {
  if(tokenList[1] == NULL){

      // lead with what caused error:
	    printf("Error: Too few arguments. Usage setpath <pathname>\n");
    } 
      else if(tokenList[2] != NULL){

	      printf("Error: Too many arguments. Usage setpath <pathname>\n"); // printing success - confused
      }
      else{
        setenv("PATH", tokenList[1], 1);
      }
}


// cd function:
void cd(char* tokenList[]) {
  // ".." automatically works - thanks UNIX!
  if(tokenList[1] == NULL){
    // no args -> HOME
    chdir(getenv("HOME"));
  }else if (tokenList[2] != NULL) {
      // Too many args
      printf("Error: Too many arguments. Usage cd <directory>\n");
  }else if(chdir(tokenList[1]) != 0){
  // Assignemnt failed
	perror(tokenList[1]);
  }
}


void saveHistory(){
  FILE *fptr;
  chdir(getenv("HOME"));
  fptr = fopen(".hist_list", "w"); 
  if (fptr == NULL){
    printf("Error: Failure saving History!\n"); 
  }
  fprintf(fptr, "%d\n", history_next); //save history next to first line
  fprintf(fptr, "%d\n", history_count);//save history count to second
  for(int i = 0; i < history_count; i++){
    fprintf(fptr, "%s \n", history_array[i]);
  }
  fclose(fptr);
}


void loadHistory(){
  FILE *fptr;
  char str[512];
  fptr = fopen(".hist_list", "r");
  if (fptr == NULL){
    printf("Could not open history file\n");
  }
  fscanf(fptr, "%d\n", &history_next);//load history next from first line
  fscanf(fptr, "%d\n", &history_count);// load history count from second line
  for (int i = 0; i < history_count; i++) {
    if (fgets(str, sizeof(str), fptr) != NULL) {
      str[strcspn(str, "\n")] = '\0';  // Remove newline
      strncpy(history_array[i], str, sizeof(history_array[i]) - 1);//copy file line to history array
      history_array[i][sizeof(history_array[i]) - 1] = '\0';//manually place null value
    }
  }
  fclose(fptr);
}

//chuck into array -> loop through array checking -- POTENTIAL HARD

// external commands:
void externalcommand(char* tokenList[]) {
    /* ELSE EXECUTE COMMAND AS AN EXTERNAL PROCESS: */
        pid_t pid;
        pid = fork();

        if (pid < 0) { // negative pid -> failure
            fprintf(stderr, "Fork failed\n");
            return;
        } else if (pid == 0) { // signifies child process
            execvp(tokenList[0], tokenList); // passes userinput[0] (function) and rest of string as arg
            // tokenList[0] == NULL, or cannot execute program - Failure
            perror(tokenList[0]);
            exit(1);
        } else {
            wait(NULL); // Parent waiting for child process to complete
        }
}

char* str_trim(char* str){
  if(str == NULL){
    return str;
  }
  char* start = str;
  while(*start == ' ' || *start == '\n' || *start == '\t'){
    start++; // move pointer in 
  }
  char* end = start + strlen(start)-1;
  while(end > start && (*end == ' ' || *end == '\n' || *end == '\t')){
    *end = '\0'; // null term
    end--; // move pointer back
  }
  // return trimmed string
  return start;
}

// bind alias with the actual command:
void createAlias(char* tokenList[]) {
  if(tokenList[1] == NULL){
    printf("Error: Too few arguments. Usage alias <name> <command>\n");
  }else if(tokenList[2] == NULL){
    printf("Error: Too few arguments. Usage alias <name> <command>\n");
  }else{
    // Combine all arguments after the alias name into a single command string
    char command[512] = "";
    for(int i = 2; tokenList[i] != NULL; i++){
      strcat(command, tokenList[i]);
      if(tokenList[i + 1] != NULL){
        strcat(command, " ");
      }
    }

    // Check if alias already exists
    for(int i = 0; i < 10; i++){
      if(alias_name[i] != NULL && strcmp(alias_name[i], tokenList[1]) == 0){
        removeAlias(tokenList);
	printf("Previous alias has been overwritten\n");
      }
    }

    // Find first empty spot in alias_name
    for(int i = 0; i < 10; i++){
      if(alias_name[i] == NULL){
        alias_name[i] = strdup(tokenList[1]);
        alias_command[i] = strdup(command);
        return;
      }
    }
    printf("Error: Too many aliases\n");
  }
}

// Print out the list of all aliases on system
void printAlias(){
  int c = 0;
  for(int i = 0; i < 10; i++){
    if(alias_name[i] != NULL){
      printf("%s: %s\n", alias_name[i], alias_command[i]);
    }else{
      c ++;
    }
  }
  if(c == 10){
    printf("Error: No aliases exist!\n");
  }
}


char* invokeAlias(char* tokenList[]) {
  static int recursion_depth = 0;

  for (int i = 0; i < 10; i++) {
    if (alias_name[i] != NULL && strcmp(alias_name[i], tokenList[0]) == 0) {
      if (recursion_depth >= 3) {
        printf("Error: Alias recursion limit exceeded\n");
        return "\0";
      }

      recursion_depth++;
      char* cmmd = strdup(alias_command[i]); // Duplicate to avoid modifying the original alias_command
      char* tokenized_command[512];
      int j = 0;

      // Tokenize the command
      char* token = strtok(cmmd, " ");
      while (token != NULL) {
        tokenized_command[j++] = token;
        token = strtok(NULL, " ");
      }
      tokenized_command[j] = NULL;

      // Check if the alias is a history invocation
      if (strcmp(tokenized_command[0], "!!") == 0 || (tokenized_command[0][0] == '!' && strlen(tokenized_command[0]) > 1)) {
        char* history_command = invoke_history(tokenized_command[0]);
        if (strcmp(history_command, "\n") != 0) {
          // Tokenize
          char* history_tokenized_command[512];
          int k = 0;
          char* token = strtok(history_command, " ");
          while (token != NULL) {
        history_tokenized_command[k++] = token;
        token = strtok(NULL, " ");
          }
          history_tokenized_command[k] = NULL;

          char* recursive_result = invokeAlias(history_tokenized_command);
          recursion_depth--;
          free(cmmd);
          return recursive_result != NULL ? recursive_result : history_command;
        }
      }

      // Recursively go through aliases
      char* recursive_result = invokeAlias(tokenized_command);
      recursion_depth--;
      free(cmmd);
      return recursive_result != NULL ? recursive_result : alias_command[i];
    }
  }
  return NULL; // no alias
}


void removeAlias(char* tokenList[]) {
  for(int i = 0; i < 10; i++){
    if(alias_name[i] != NULL && strcmp(alias_name[i], tokenList[1]) == 0){
      alias_name[i] = NULL;
      alias_command[i] = NULL;
      return;
    }
  }
  printf("Error: Alias not found\n");
}


void saveAlias(){
  FILE *fptr;
  chdir(getenv("HOME"));
  fptr = fopen(".aliases", "w"); 
  if (fptr == NULL){
    printf("Error: Failure saving aliases!\n"); // perror
  }
  for(int i = 0; i < 10; i++){
    if(alias_name[i] != NULL){
      fprintf(fptr, "%s %s\n", alias_name[i], alias_command[i]);
    }
  }
  fclose(fptr);
}

void loadAlias(){
  FILE *fptr;
  char str[512];
  fptr = fopen(".aliases", "r");
  if (fptr == NULL){
    printf("Could not open alias file\n");
  }
  for (int i = 0; i < 10; i++) {
    if (fgets(str, sizeof(str), fptr) != NULL) {
      str[strcspn(str, "\n")] = '\0';  // Remove newline
      char* name = strtok(str, " ");
      char* command = strtok(NULL, "\n");
      alias_name[i] = strdup(name);
      alias_command[i] = strdup(command);
    }
  }
  fclose(fptr);
}

void clearAlias(){
  for(int i = 0; i < 10; i++){
    alias_name[i] = NULL;
    alias_command[i] = NULL;
  }
  // Delete the alias file if it exists
  chdir(getenv("HOME"));
  if (access(".aliases", F_OK) == 0) {
    remove(".aliases");
  }
}

void clearHistory(){
  for(int i = 0; i < HISTORY_SIZE; i++){
    history_array[i][0] = '\0'; // clear all history
  }
  history_count = 0;
  history_next = 0;
  // Delete the history file if it exists
  chdir(getenv("HOME"));
  if (access(".hist_list", F_OK) == 0) {
    remove(".hist_list");
  }
}
