/*
    程序名：webserv_linux.c
    功能：该程序为多线程Web服务器的服务器端，使用标准I/O函数，只是为了复习各种知识点
        基于HTTP协议，以浏览器(建议firefox)为客户端访问该服务器的html文件。
    作者：fouries
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

// 定义缓冲器 buf和 req_line的大小
#define BUF_SIZE 1024
#define SMALL_BUF 100

void* request_handler(void *arg);
void send_data(FILE* fp, char* ct, char* file_name);
char* content_type(char* file);
void send_error(FILE* fp);
void error_handling(char* message);

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_size;
    char buf[BUF_SIZE];
    pthread_t t_id;
    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if(listen(serv_sock, 20) == -1)
        error_handling("listen() error");
    
    while(1)
    {
        clnt_adr_size = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_size);
        printf("Connection Request : %s:%d\n", inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
        // 创建一个线程，从request_handler函数调用开始，在单独的执行流中运行。同时在调用request_handler函数时向其传递clnt_sock变量的地址值。
        pthread_create(&t_id, NULL, request_handler, &clnt_sock);
        // 摧毁一个线程， pthread_detach()不会引起堵塞
        pthread_detach(t_id);
    }
    close(serv_sock);
    return 0;
}

//  请求处理函数，请求处理线程的main函数
void* request_handler(void *arg)
{
    int clnt_sock = *((int*)arg);
    // 定义数组req_line用于保存请求行数据
    char req_line[SMALL_BUF];
    // 定义FILE*类型变量 clnt_read、clnt_write
    FILE* clnt_read;
    FILE* clnt_write;

    // 分别定义char型数组method、ct、file_name用于保存请求方法、内容类型、文件名
    char method[10];
    char ct[15];
    char file_name[30];

    // 以读的模式打开 clnt_sock
    clnt_read = fdopen(clnt_sock, "r");
    // 以写的模式打开 clnt_sock， dup函数创建一个新的文件描述符，该文件描述符和原有文件描述符指向相同的文件
    clnt_write = fdopen(dup(clnt_sock), "w");
    // 从clnt_read流中读取SMALL_BUF长度的字符串到 req_line数组
    fgets(req_line, SMALL_BUF, clnt_read);
    // 若 req_line中没有 "HTTP/"子串
    if(strstr(req_line, "HTTP/")==NULL)
    {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return;
    }
    // strtok(char *str, const char *delim)；以delim为分隔符分割str,返回被分解的第一个子字符串
    // 将req_line中的请求方法字符串保存于method数组中
    strcpy(method, strtok(req_line, " /"));
    // 将req_line中的文件名字符串保存于file_name数组中
    strcpy(file_name, strtok(NULL, " /"));
    // 将文件内容的类型保存于ct数组中
    strcpy(ct, content_type(file_name));
    // 判断 method是否为 GET
    if(strcmp(method, "GET")!=0)
    {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return;
    }
    // 关闭clnt_read文件
    fclose(clnt_read);
    // 发送数据
    send_data(clnt_write, ct, file_name);
}

// 将头消息发送到 流fp
void send_data(FILE* fp, char* ct, char* file_name)
{
    // 状态行
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    // 消息头
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[SMALL_BUF];
    // 消息缓冲区
    char buf[BUF_SIZE];
    FILE* send_file;

    // 将"Content-type:%s\r\n\r\n"和 ct输入到 cnt_type
    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    // 以读的模式打开 file_name， 并且赋值给 send_file
    send_file = fopen(file_name, "r");
    
    // 如果打开失败
    if(send_file == NULL)
    {
        send_error(fp);
        return;
    }

    /*传输头信息*/
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    /*传输请求数据*/
    while(fgets(buf, BUF_SIZE, send_file)!=NULL)
    {
        fputs(buf, fp);
        fflush(fp);
    }
    // 刷新流fp的输出缓冲区
    fflush(fp);
    // 关闭流fp
    fclose(fp);
}

// 判断文件内容类型的函数
char* content_type(char* file)
{
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    // 得到 file的文件名，保存于 file_name中
    strcpy(file_name, file);
    strtok(file_name, ".");
    // 得到 file的扩展名保存于 extension中
    strcpy(extension, strtok(NULL, "."));
    // 判断file的扩展名是否为 html或者htm
    if(!strcmp(extension, "html")||!strcmp(extension, "htm"))
        return "text/html";
    else
        return "text/plain";
}

// http请求发生错误，发送错误信息函数
void send_error(FILE* fp)
{
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
        "<body><font size =+5><br>发生错误！ 查看请求文件名和请求方式！"
        "</font></body></html>";

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fflush(fp);
}

// 错误处理函数
void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}