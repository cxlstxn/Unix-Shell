#include <stdio.h>
#include <unistd.h>

int main() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s$", cwd);
    int num;
    char user_input[512];
    int result = scanf("%d", &num);
}