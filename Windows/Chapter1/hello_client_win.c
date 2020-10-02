#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
void Error_Handling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen;
	if(argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if( WSAStartup(MAKEWORD(2, 2), &wsaData) != 0 ) // 初始化Winsock库。
		Error_Handling("WSAStartup() error");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);		 // 创建套接字，第32行通过此套接字向服务器端发出连接请求。
	if(hSocket == INVALID_SOCKET)
		Error_Handling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) // 功能同上
		Error_Handling("connet() error!");

	strLen = recv(hSocket, message, sizeof(message)-1, 0);  // 调用recv函数接收服务器发来的数据。
	if(strLen == -1)
		Error_Handling("read() error!");
	printf("Message from server : %s \n", message);
	
	closesocket(hSocket);
	WSACleanup();											 // 注销第20行中初始化的Winsock库。
	return 0;
}

void Error_Handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}