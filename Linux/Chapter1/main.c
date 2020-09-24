#include <stdio.h>

//主函数的第二个参数是一个字符指针数组。其中每个指针代表一个字符串，所有这写字符串的内容都来字用户的命令，主函数的第一个参数是一个整数，表示第二个参数里指针的个数,这里的参数名可以改
int main(int argc, char *argv[]) { 
    int num;
    printf("%d parameters\n", argc+1);
    for(num = 0; num <= argc; num++) {
        printf("parameter %d: %s\n", num ,argv[num]);
    }
    return 0;
}