/*
    程序名：chat_server.c
    功能：为多个聊天客户端提供服务
        使用用多线程和锁机制（互斥量）, 重点掌握临界区的构成。
        “访问全局变量clnt_cnt和数组clnt_socks的代码将构成临界区！”
    作者：fouries
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void *handle_clnt(void *arg);
void send_msg(char *msg, int len);
void error_handling( char *message);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;                           // 声明pthread_mutex_t型变量，用来保存操作系统创建的互斥量（锁系统）。

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_sz;
    pthread_t t_id;                             // 创建一个pthread_t类型变量t_id，用于保存新创建线程ID的变量地址值。
    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 创建互斥量
    pthread_mutex_init(&mutx, NULL);
    
    // 创建socket套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    // 初始化sockaddr_in结构体类型的变量serv_adr
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    // 给套接字分配地址
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error!");
    
    // 使serv_sock进入可连接状态, 连接请求等待队列长度为5。
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error!");

    while(1)
    {
        clnt_adr_sz = sizeof(clnt_adr);
        // 受理客户端的连接请求
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

        // 利用互斥量锁住并释放临界区
        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++] = clnt_sock;     // 临界区， 将受理连接的客户端文件描述符保存于clnt_socks数组中
        pthread_mutex_unlock(&mutx);

        // 创建一个线程，从handle_clnt函数调用开始，在单独的执行流中运行。同时在调用handle_clnt函数时向其传递clnt_sock变量的地址值。
        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);

        // pthread_join:调用该函数的线程将进入阻塞状态，等待结束后回收其资源
        // pthread_detach:调用该函数不会引起线程阻塞，线程结束后会自动释放所有资源
        pthread_detach(t_id);
        printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
    }
    // 关闭服务器端套接字
    close(serv_sock);
    return 0;
}

// t_id线程的main函数，给客户端提供服务的函数
void *handle_clnt(void *arg)
{
    int clnt_sock = *((int*)arg);  // handle_clnt的参数为主进程的clnt_sock变量，类型为void*
    int str_len = 0, i;
    char msg[BUF_SIZE];

    // 接收客户端发送过来的消息，并给所有客户端都发送该消息
    while((str_len=read(clnt_sock, msg, sizeof(msg))) != 0)
        send_msg(msg, str_len);

// 利用互斥量锁住并释放临界区
    pthread_mutex_lock(&mutx);
    // 清除无连接的客户端
    // 临界区起始
    for(i=0; i<clnt_cnt; i++)
    {
        // 遍历整个clnt_socks数组，当传给进程的套接字的文件描述符与连接上的套接字的文件描述符相等时
        if(clnt_sock == clnt_socks[i]) 
        {
            // 将clnt_socks数组从索引i+1到clnt_cnt-1的套接字文件描述符整体向前移动一位（覆盖了重复的clnt_socks[i]）。i最大为clnt_clnt-1
            while(i++<clnt_cnt-1)
                clnt_socks[i] = clnt_socks[i+1];
            break;      // 退出循环
        }
    }
    clnt_cnt--;         // 清除了clnt_socks里一个套接字之后，将clnt_cnt存储的数量减 1
    // 临界区结束
    pthread_mutex_unlock(&mutx);
    
    // 关闭客户端的套接字
    close(clnt_sock);               
    return NULL;    
}

// 给所有客户端发送消息的函数
void send_msg(char *msg, int len)   
{
    int i;

    // 利用互斥量锁住并释放临界区
    pthread_mutex_lock(&mutx);
    for(i=0; i<clnt_cnt; i++)               // 临界区
        write(clnt_socks[i], msg, len);     // 临界区
    pthread_mutex_unlock(&mutx);
}

void error_handling( char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}