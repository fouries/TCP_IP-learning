/*
    程序名：echo_mpserver.c
    功能：该程序为多线程回声服务器的服务器端
        使用信号处理器清除终止了的子进程，多线程用于受理客户端的的连接请求
    作者：fouries
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;

    pid_t pid;
    struct sigaction act;   // 定义struct sigaction结构体变量act
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];
    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 初始化struct sigaction结构体变量act
    act.sa_handler = read_childproc; 
    sigemptyset(&act.sa_mask);  // act的struct sigset_t结构体类型变量sa_mask所有位初始化为0
    act.sa_flags = 0;           // act的int型成员变量sa_flags初始化为0

    // 注册信号，成功时返回0，失败时返回-1。
    state = sigaction(SIGCHLD, &act, 0);

    // 调用socket()函数，创建套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
    // 初始化sockaddr_in结构体类型的变量serv_adr
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    // 调用bind()函数，为套接字分配地址
    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error!");
    
    // 调用listen()函数，使serv_sock进入可连接状态, 连接请求等待队列长度为5。
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error!");
    
    while(1)
    {
        adr_sz = sizeof(clnt_adr);
        // 调用accept()函数，受理客户端的连接请求
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock == -1)
            continue;   // 受理错误，退出重新进入while循环
        else
            puts("new client connected...");
        
        // 创建一个子进程，如果子进程创建失败，则关闭clnt_sock套接字，退出重新进入while循环
        pid = fork();   
        if(pid == -1)   
        {
            close(clnt_sock);
            continue;
        }

        if(pid == 0)    /*子进程运行区域*/
        {
            close(serv_sock);   // 关闭子进程中复制来的serv_sock套接字

            // 接收客户端发送的数据，并发送给客户端
            while((str_len=read(clnt_sock, buf, BUF_SIZE)) != 0)
                write(clnt_sock, buf, str_len);
            
            // 收发完毕，关闭clnt_sock套接字
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
        else    // 关闭父进程中的clnt_sock套接字
            close(clnt_sock);
    }
    close(serv_sock);    // 最后关闭父进程serv_sock套接字
    return 0;
}

// 信号处理器，清除终止的子进程，防止僵尸进程。
void read_childproc(int sig)
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);  // 调用waitpid函数清除终止的子进程，返回终止子进程的ID
    printf("removed proc id: %d \n", pid);      // 打印被移除进程的ID
}

// 错误处理函数
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}