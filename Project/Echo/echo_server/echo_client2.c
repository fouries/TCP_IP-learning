/*
    程序名：echo_client2.c
    功能：基于TCP的回声客户端2
         该客户端完美解决了发送数据量大时数据丢失的问题
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
    int str_len, recv_len, recv_cnt;
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
            break;`
        
        // Step3： 调用write()/read()函数，进行数据交换
        str_len = write(sock, message, strlen(message));
        recv_len = 0;

        // 判断接收到的消息的长度是否等于发送了的消息的长度，当相等时退出while。（用 ！= 判断，容易引发异常造成无限循环，用 < 即使异常也不会无限循环）
        while(recv_len < str_len)
        {
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE-1);
            if(recv_cnt == -1)
                error_handling("read() error!");
            recv_len += recv_cnt;
        }

        message[recv_len] = 0;   // 等价于 = '\0';
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

