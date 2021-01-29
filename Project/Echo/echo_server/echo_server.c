/*
    程序名：echo_server.c
    功能：基于TCP的回声服务器端
    作者：fouries
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // Step1: 调用socket()函数，创建服务器端套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error!");

    // 初始化sockaddr_in结构体类型的变量serv_adr
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    
    // Step2: 调用bind()函数，给套接字分配地址
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error!");

    // Step3: 调用listen()函数，使serv_sock进入可连接状态, 连接请求等待队列长度为5。（最多让5个连接请求进入队列）
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error!");

    clnt_adr_sz = sizeof(clnt_adr);
    
    for(i=0; i<5; i++)
    {   
        // Step4: 调用accept()函数，受理客户端的连接请求
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1)
            error_handling("accept() error!");
        else
            printf("Connected client %d \n", i+1);
        
        // Step5: 调用read()/write()函数进行数据交换
        while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
            write(clnt_sock, message, str_len);

        close(clnt_sock);
    }

    // Step6: 调用close()函数关闭服务器端套接字
    close(serv_sock);
    return 0;
}

void error_handling( char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}