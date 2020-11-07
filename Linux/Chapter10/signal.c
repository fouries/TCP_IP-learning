#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig == SIGALRM)
        puts("Time out!");
    alarm(4);
}

void keycontrol(int sig)
{
    if(sig == SIGINT)
        puts("Cotol+C pressed");
}

int main(int argc, char* argv[])
{
    int i;
    signal(SIGALRM, timeout);       // 函数名即为函数指针
    signal(SIGINT, keycontrol);     
    alarm(4);
    for(i=0; i<3; i++)
    {
        puts("wait...");
        sleep(100);
    }
    
    return 0;
}