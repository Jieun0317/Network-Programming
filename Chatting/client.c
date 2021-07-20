
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
void ErrorHandling(char* message);

#pragma comment(lib, "ws2_32.lib")

void printMsgs(SOCKET hSocket, SOCKADDR_IN servAddr, char *msgs) {
	send(hSocket, "##GETMSG##", strlen("##GETMSG##"), 0);
	// 单捞磐 荐脚
	recv(hSocket, msgs, 10000, 0);
	closesocket(hSocket);
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	char id[10];
	char msgs[10000] = "";
	char message[30];
	char sendmsg[40];
	int strLen;
	int str_len = 40;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	// 家南 积己

	printf("Input ID : ");
	scanf("%s",id);

	while (1) {
		system("cls");

		hSocket = socket(PF_INET, SOCK_STREAM, 0);
		if (hSocket == INVALID_SOCKET)
			ErrorHandling("hSocketet() error");

		//匙飘况农 林家 汲沥
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		servAddr.sin_port = htons(atoi("50000"));

		if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
			ErrorHandling("connect() error!");

		printMsgs(hSocket, servAddr, msgs);

		printf("%s\n", msgs);
		memset(&message, 0, sizeof(message));
		fputs("Input message(Q to quit & R to refresh) : ", stdout);
		fgets(message, str_len, stdin);

		if (strlen(message) <= 1) continue;
		if (strcmp(message, "r\n") == 0 || strcmp(message, "R\n") == 0) continue;
		if (strcmp(message, "q\n") == 0 || strcmp(message, "Q\n") == 0) break;

		strcpy(sendmsg, id);
		strcat(sendmsg, ": ");
		strcat(sendmsg, message);

		hSocket = socket(PF_INET, SOCK_STREAM, 0);
		if (hSocket == INVALID_SOCKET)
			ErrorHandling("hSocketet() error");

		//匙飘况农 林家 汲沥
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		servAddr.sin_port = htons(atoi("50000"));

		if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
			ErrorHandling("connect() error!");

		send(hSocket, sendmsg, str_len - 1, 0);
		closesocket(hSocket);
	}

	// 家南 辆丰
	WSACleanup();
	getchar();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}