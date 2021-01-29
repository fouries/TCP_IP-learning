/*
    程序名：echo_client.c
    功能：基于TCP的回声客户端
        该客户端56-59行存在问题。
            错误1.每次调用read、write函数时都会以字符串为单位执行实际的I/O操作
            错误2.字符串太长，需要分2个数据包发送！
    作者：fouries
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling( char *message);

int main(int argc, char* argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    if(argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // Step1： 调用socket()函数，创建客户端套接字
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error!");

    // sockaddr_in结构体类型的变量 serv_adr初始化
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    // Step2: 调用connect()函数，请求与服务器端连接
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected.....");
    
    while(1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        
        // Step3： 调用write()/read()函数，进行数据交换
        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF_SIZE-1);
        message[str_len] = 0;   // 等价于 = '\0';
        printf("Message from server: %s", message);
    }
    
    // Step4: 调用close()函数，关闭客户端套接字
    close(sock);
    return 0;
}

void error_handling( char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
