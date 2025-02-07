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


### To-do:
- Implement Stage 3
- Clean up code