#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    unsigned short host_port = 0x1234;                      // 各保存2个字节、4个字节的数据。当然，若运行程序的CPU不同，则保存的字节序也不同
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;                   // 各保存2个字节、4个字节的数据。当然，若运行程序的CPU不同，则保存的字节序也不同
    unsigned long net_addr;

    net_port = htons(host_port);                            // 变量host_port和host_addr中的数据转化为网络字节序。若运行环境为小端序CPU，则按改变之后的字节序保存。
    net_addr = htonl(host_addr);                            
    printf("Host ordered port: %#x \n", host_port);
    printf("Network ordered port: %#x \n", net_port);
    printf("Host ordered address: %#lx \n", host_addr);
    printf("Network ordered address: %#lx \n", net_addr);
    return 0;
}
