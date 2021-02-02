/*
    程序名：chat_clnt.c
    功能：多人聊天室的客户端
         客户端为了分离输入和输出过程而创建了线程。
    作者：fouries
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void *send_msg(void *arg);
void *recv_msg(void *arg);
void error_handling( char *message);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_adr;
    pthread_t snd_thread, rcv_thread;
    void *thread_return;
    if(argc != 4)
    {
        printf("Usage : %s <IP> <port> <name>\n", argv[0]);
        exit(1);
    }

    sprintf(name, "[%s]", argv[3]);
    // 创建客户端套接字
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error!");

    // sockaddr_in结构体类型的变量 serv_adr初始化
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    // 请求与服务器端连接
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    
    // 分别创建snd_thread和rcv_thread线程
    pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
    pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
    // 分别等待snd_thread和rcv_thread线程结束，thread_return保存线程的main函数返回值
    pthread_join(snd_thread, &thread_return);
    pthread_join(rcv_thread, &thread_return);

    // 关闭客户端套接字
    close(sock);
    return 0;
}

// snd_thread线程的main函数,用于客户端发送数据
void *send_msg(void *arg)
{
    int sock = *((int*)arg);
    // 声明字符数组name_msg，保存姓名字符串和消息字符串
    char name_msg[NAME_SIZE+BUF_SIZE];
    while(1)
    {   
        // 接收客户端从终端输入的字符串消息，保存于msg字符数组中
        fgets(msg, BUF_SIZE, stdin);
        // 判断输入的字符串是否是“Q\n”或“q\n”
        if(!strcmp(msg, "q\n")||!strcmp(msg, "Q\n"))
        {
            close(sock);
            exit(0);        // 立即终止调用进程（关掉主进程的I/O），并向主进程返回 0。
        }
        // 将name和msg数组中的数据以字符串的格式输出到name_msg数组中
        sprintf(name_msg, "%s %s", name, msg);
        // 将name_msg数组中的字符发送到服务器端套接字
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

// rcv_thread线程的main函数，用于接收数据
void *recv_msg(void *arg)
{
    int sock = *((int*)arg);
    // 声明字符数组name_msg，保存姓名字符串和消息字符串
    char name_msg[NAME_SIZE+BUF_SIZE];
    int str_len;
    while(1)
    {
        // 接收服务器端发送的数据，并将数据保存于 name_msg之中
        str_len = read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
        // 如果接收失败，则该线程返回(void*)类型的-1
        if(str_len == -1)
            return (void*)-1;
        // 字符数组末尾加 '\0'
        name_msg[str_len] = 0;
        // 输出格式化后的姓名和消息
        fputs(name_msg, stdout);
    }
    return NULL;
}

void error_handling( char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

