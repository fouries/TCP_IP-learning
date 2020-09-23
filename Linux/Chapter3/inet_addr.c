#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char * argv[])
{
    char *addr1 = "1.2.3.4";
    char *addr2 = "1.2.3.255";                                         // 一个字节能表示的最大整数为255，它是错误的IP地址。

    unsigned long conv_addr = inet_addr(addr1);                        // 通过运行结果验证第9行的函数正常调用，而第15行的函数调用出现异常。
    if(conv_addr == INADDR_NONE)
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr: %#lx \n", conv_addr);    // %#lx以十六进制、带前缀、无符号的形式输出long类型的整数

    conv_addr = inet_addr(addr2);
    if(conv_addr == INADDR_NONE)
        printf("Error occureded \n");
    else
        printf("Network ordered integer addr: %#lx \n\n", conv_addr);
    return 0;
}