#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024                                                      // 将待算数字的字节数和运算结果的字节数设为常数
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char opmsg[BUF_SIZE];                                                  // 为收发数据准备的内存空间，需要数据积累到一定程度后再收发，因此通过数组创建
    int result, opnd_cnt, i;
    struct sockaddr_in serv_adr;

    if(argc!=3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock=socket(PF_INET, SOCK_STREAM, 0);
    if(sock==-1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("connect() error");
    else
        puts("Connected...........");
    
    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;                                              // 从程序用户的输入中得到待算个数后，保存至数组opmsg。强制转换成char类型，因为协议规定待算个数
                                                                            // 应通过1个字节整数型传递，因此不能超过1字节整数型能够表示的范围。该示例中用的是有符号整数型，
    for(i=0; i<opnd_cnt; i++)                                               // 但待算数个数不能是负数，因此使用无符号整数型更合理。
    {
        printf("Operand %d: ", i+1);
        scanf("%d", (int*)&opmsg[i*OPSZ+1]);
    }
    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opnd_cnt*OPSZ+1]);
    write(sock, opmsg,opnd_cnt*OPSZ+2);
    read(sock, &result, RLT_SIZE);

    printf("Operation result: %d \n", result);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}