#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int x = 1; 
    while (x==1) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s>$", cwd);
        char result[100]; 
        fgets(result, sizeof(result), stdin);
        char* token = strtok(result, " < \t | > & ;");
        while (token != NULL) {
            printf(" %s\n", token);
            token = strtok(NULL, " < \t | > & ;");
        }
    }
}