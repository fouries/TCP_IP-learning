/*
    程序名：echo_mpclient.c
    功能：该程序为多线程进程回声客户端
        回声客户端无特殊原因不需要分割I/O, 本程序用于讲解I/O分割的方法而选择回声客户端
        对回声客户端的读/写数据进行了分割。
    作者：fouries
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling( char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char* argv[])
{
    int sock;
    pid_t pid;
    char buf[BUF_SIZE];
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
    
    // Step3: 调用write_routine/read_routine函数收发数据
    pid = fork();
    if(pid == 0)
        write_routine(sock, buf);
    else
        read_routine(sock, buf);
    
    // Step4: 调用close()函数，关闭客户端套接字
    close(sock);
    return 0;
}


// 分割I/O后的客户端接收数据函数
void read_routine(int sock, char *buf)
{
    while(1)
    {   
        // 读取服务器端返回的消息，并打印出来
        int str_len = read(sock, buf, BUF_SIZE);
        if(str_len == 0)
            return;

        buf[str_len] = 0;
        printf("Message from server: %s", buf);
    }
}

// 分割I/O后的客户端发送数据函数
void write_routine(int sock, char *buf)
{
    while(1)
    {
        // 输入需要发送的消息，并发送给服务器端
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
        {
            shutdown(sock, SHUT_WR);    // TCP的半关闭，断开输出流，向服务器端发送EOF，还能用read_routine()接收数据
            return;                     // 返回空，void返回类型也可以什么都不写
        }
        write(sock, buf, strlen(buf));  // 半关闭之后无法发送数据
    }
}

void error_handling( char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
