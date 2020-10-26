#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sd, clnt_sd;
    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt;
    socklen_t clnt_addr_sz;
    
    struct sockaddr_in serv_addr, clnt_addr;
    
    if(argc != 2)
    {
        printf("Usege: %s <port>\n", argv[0]);
        exit(1);
    }

    fp = fopen("file_server.c", "rb");
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sd == -1)
        error_handling("socket() error!");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(serv_sd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    listen(serv_sd, 5);

    clnt_addr_sz = sizeof(clnt_addr);
    clnt_sd = accept(serv_sd, (struct sockaddr*) &clnt_addr, &clnt_addr_sz);

    while(1)
    {
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if(read_cnt < BUF_SIZE)     // 如果读出的字节数小于 BUF_SIZE， 调用write(clnt_sd, buf, read_cnt);
        {
            write(clnt_sd, buf, read_cnt);
            break;                  // break 跳出循环， continue跳过本次循环的代码，开始下一次循环
        }
        write(clnt_sd, buf, BUF_SIZE);
    }

    shutdown(clnt_sd, SHUT_WR);
    read(clnt_sd, buf, BUF_SIZE);  // 关闭输出流，依然可以通过输入流接收数据
    printf("Message from client: %s \n", buf);

    fclose(fp);
    close(clnt_sd); close(serv_sd);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}