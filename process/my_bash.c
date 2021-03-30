#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 256
#define ARGS_BUF_SIZE 64
#define ARGS_DELIM " \t\r\n\a"

char **split_args(char *str)
{
    int bufsize = ARGS_BUF_SIZE;
    int position = 0;
    char **args = malloc(bufsize * sizeof(char*));
    char *token;

    if (!args) {
        fprintf(stderr, "my_shell: ошибка выделения памяти");
        exit(EXIT_FAILURE);
    }

    token = strtok(str, ARGS_DELIM);
    while (token != NULL) {
        args[position] = token;
        position++;
    
        if (position >= bufsize) {
            bufsize += ARGS_BUF_SIZE;
            args = realloc(args, bufsize * sizeof(char*));
            if (!args) {
                fprintf(stderr, "my_shell: ошибка выделения памяти");
                exit(EXIT_FAILURE);
            }
        }
        
        token = strtok(NULL, ARGS_DELIM);
    }

    args[position] = NULL;
    return args;
}

int exit_cmp(char *str)
{
    return strcmp(str, "exit");
}

int main(int argc, char *argv[])
{
    char name_bin[BUF_SIZE];
    char curr_dir[BUF_SIZE];
    int child_pid;
    int wstatus;
    int ret;

    do {
        printf("%s$ ", getcwd(curr_dir, sizeof(curr_dir)));
        fgets(name_bin, BUF_SIZE, stdin);
        name_bin[strlen(name_bin) - 1] = '\0'; 

        switch (child_pid = fork()) {
            case -1:
                return -1;

            case 0:
                ret = execvp(name_bin, split_args(name_bin));
            
                if (ret == -1 && exit_cmp(name_bin) != 0) {
                    printf("unknown command\n");
                    exit(1);
                }
                
            default:
                if (waitpid(child_pid, &wstatus, 0) == -1)
                    return -1;
        }
    } while (exit_cmp(name_bin));

    return 0;
}