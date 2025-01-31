#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int x = 1; 
    while (x == 1) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s>$", cwd);
        char result[100]; 
        fgets(result, sizeof(result), stdin);
        
        if (feof(stdin)) {
            break;
        }
        if (strcmp(result, "exit\n") == 0) {
            break;
        }

        char* tokens[100];
        int token_count = 0;
        char* token = strtok(result, " < \t | > & ;");
        while (token != NULL) {
            char *newline = strchr(token, '\n');
            if (newline) {
            *newline = '\0';
            }
            tokens[token_count++] = token;
            token = strtok(NULL, " < \t | > & ;");
        }
        
        for (int i = 0; i < token_count; i++) {
            printf("'%s'\n", tokens[i]);
        }
    }
}