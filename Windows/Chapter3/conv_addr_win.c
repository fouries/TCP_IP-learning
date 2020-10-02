#undef UNICODE
#undef _UNICODE
#include <stdio.h>
#include <winsock2.h>

// WSAAddressToString与WSAStringToAddress在功能上正好相反，它将结构体中的地址信息转换成字符串形式。
// 在功能上与inet_ntoa和inet_addr完全相同，但优点在于支持多种协议，在IPv4和IPv6中均可适用。
int main(int argc, char *argv[])
{
	char *strAddr = "203.211.218.102:9190";

	char strAddrBuf[50];
	SOCKADDR_IN servAddr;
	int size;

	WSADATA	wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	size = sizeof(servAddr);
	WSAStringToAddress(
		strAddr, AF_INET, NULL, (SOCKADDR*)&servAddr, &size);

	size = sizeof(strAddrBuf);
	WSAAddressToString(
		(SOCKADDR*)&servAddr, sizeof(servAddr), NULL, strAddrBuf, &size);

	printf("Second conv result: %s \n", strAddrBuf);
	WSACleanup();
	return 0;
}