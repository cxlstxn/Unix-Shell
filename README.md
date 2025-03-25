# CS210 - Computer Systems and Architecture - Simple Shell Practical 
## Authors: Fraser Watt, Alex Boyd, Ross Paton, Jacob Mwangu, Finlay Colston


## Stage Summary:

### Stage 1:
- Print shell like prompt - including the user's current working directory
- Be able to accept and tokenise user input
- Be able to exit shell using both 'exit' and CRTL+D respectively

### Stage 2:
- Search through current internal commands ('exit' & CTRL+D) and execute them if appropriate
- If external command:
  - Fork process and create a child process
  - Search through the user PATH and find if there is a corresponding command - if so execute said command - otherwise, print error message informing the user of invalid input

### Stage 3:
- Store the original user path
- Restore original user path upon exiting the program
- Change user environment to user/home when shell starts up
- Add getpath and setpath as internal commands:
  - ```getpath``` - Displays the current user path
  - ```setpath``` - Sets the environment path to valid user input

### Stage 4:
- Implement the ```cd``` comamnd - allows user to change current working directory
- 3 types:
  - no args - returns user to home folder
  - ```..``` - sets directory to parent folder
  - ```filepath``` - sets directory to requested filepath if valid 

### Stage 5:
- Implemented History, 20 commands wrapping around when overflow.
  - ```history``` - Returns a list of up to 20 of the last commands. 

### Stage 6:
- Implemented saving history, On start the shell loads history from a file. 


## Overview:

```
Find the user home directory from the environment
Set current working directory to user home directory
Save the current path
Load history
Load aliases
Do while shell has not terminated
   Display prompt
   Read and parse user input
   While the command is a history invocation or alias then replace it with the appropriate command from history or the aliased command respectively  
   If command is built-in invoke appropriate function
   Else execute command as an external process
End while
Save history
Save aliases
Restore original path
Exit
```

## Running the Shell:
- Use:
```
. build.txt
```
- to build shell.
```
./shell
```
- to run the shell. 



### To-do:
- Start Stage 7. 
- When input is > 511 anything after is treated as a new command, this should not happen and should print an error instead.
-