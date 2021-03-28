#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 256

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
            case -1:    /* ошибка */
                return -1;

            case 0:     /* потомок */
                ret = execlp(name_bin, "", (char *) NULL);
            
                if (ret == -1 && exit_cmp(name_bin) != 0) {
                    printf("unknown command\n");
                    exit(1);
                }
                
            default:    /* родитель */
                if (waitpid(child_pid, &wstatus, 0) == -1)
                    return -1;
        }
    } while (exit_cmp(name_bin));

    return 0;
}