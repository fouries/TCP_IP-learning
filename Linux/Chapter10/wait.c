#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int status;
    pid_t pid = fork();

    if(pid==0)
    {
        return 3;
    }
    else
    {
        printf("Child PID: %d \n", pid);
        pid = fork();
        if(pid==0)
        {
            exit(7);
        }
        else
        {
            printf("Child PID: %d \n", pid);
            int pid1 = wait(&status);
            if(WIFEXITED(status))
                printf("Child send one: %d \n", WEXITSTATUS(status));
            printf("Child PID1: %d, status is %d\n", pid1, status);

            int pid2 =wait(&status);
            if(WIFEXITED(status))
                printf("Child send two: %d \n", WEXITSTATUS(status));
            printf("Child PID2: %d, status is %d\n", pid2, status);
            sleep(30);
        }
    }

    return 0;
}