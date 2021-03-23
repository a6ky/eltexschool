/* Программа создает следующую иерархию процессов:  
 *
 *                      1
 *                     / \
 *                    2   3
 *                   / \   \
 *                  4   5   6
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void wait_for_any_child()
{
    int wstatus;
    pid_t child_pid = wait(&wstatus);
    if (child_pid == -1)
        abort();
    printf("pid: %d terminated\n", child_pid);       
}

int main(int argc, char *argv[])
{
    printf("p1 started with pid: %d ppid: %d\n", getpid(), getppid());

    if (fork() == 0) {  // p2        
        if (fork() == 0) {  // p4
            printf("p4 started with pid: %d ppid: %d\n", getpid(), getppid());
        } else {
            if (fork() == 0) {  // p5
                printf("p5 started with pid: %d ppid: %d\n", getpid(), getppid());
            } else {
                printf("p2 started with pid: %d ppid: %d\n", getpid(), getppid());
                wait_for_any_child();
                wait_for_any_child();
            }
        }
    } else {    
        if (fork() == 0) {  // p3
            if (fork() == 0) {  // p6
                printf("p6 started with pid: %d ppid: %d\n", getpid(), getppid());
            } else {
                printf("p3 started with pid: %d ppid: %d\n", getpid(), getppid());
                wait_for_any_child();
            }
        } else { //p1
            wait_for_any_child();
            wait_for_any_child();
            printf("pid: %d terminated\n", getpid());   
        }
    }

    exit(EXIT_SUCCESS);
}
