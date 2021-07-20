#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
void ErrorHandling(char* message);

#pragma comment(lib,"ws2_32.lib")


int main(int argc, char* argv[])
{
	WSADATA	wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int str_len;

	int szClntAddr;
	char message[40] = "Hello World!";
	char msgs[10000] = "";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	// 소켓 생성
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	// 네트워크 주소 설정
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi("50000"));

	// 주소 할당
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	// 소켓 오픈
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");


	while (1) {
		// 소켓 연결 수락	
		szClntAddr = sizeof(clntAddr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
		if (hClntSock == INVALID_SOCKET)
			ErrorHandling("accept() error");


		// 데이터 송수신
		while (1) {
			memset(&message, 0, sizeof(message));

			str_len = recv(hClntSock, message, sizeof(message), 0);
			if (str_len != 0) {
				if (strcmp(message, "##GETMSG##") == 0) {
					send(hClntSock, msgs, strlen(msgs), 0);
				}
				else {
					printf("%s:%d >> %s", inet_ntoa(clntAddr.sin_addr), (int)ntohs(clntAddr.sin_port),  message);
					strcat(msgs, message);
				}
			}
			break;
		}

		// 클라이언트 소켓 종료
		closesocket(hClntSock);
	}

	// 서버 소켓 종료
	closesocket(hServSock);

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
