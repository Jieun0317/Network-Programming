#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"wsock32.lib")

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <stdlib.h>
#include <time.h>

#define PORT 5500

int main() {
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	
	SOCKET c_Socket = socket(AF_INET, SOCK_STREAM, 0);//클라이언트 소켓 생성

	if (c_Socket == INVALID_SOCKET) {
		printf("error\n");
		return 0;
	}
	printf("클라이언트 소켓 생성 완료했습니다.\n\n");

	SOCKADDR_IN serverAddress;//주소 정보 구조체
	ZeroMemory(&serverAddress, 0, sizeof(serverAddress));//구조체 초기화

	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);

	connect(c_Socket, (SOCKADDR*)& serverAddress, sizeof(serverAddress));

	printf("로또 번호 맞추기를 시작합니다.\n\n");

	while (1) {
		int number[6];

		printf("6개의 로또 번호를 입력해 주세요.(범위 : 1 ~ 45) : ");
		scanf_s("%d %d %d %d %d %d", &number[0], &number[1], &number[2], &number[3], &number[4], &number[5]);
	
		for (int i = 0; i < 6; i++)
			number[i] = htonl(number[i]);
		
		send(c_Socket, (char*)number, sizeof(number), 0);//입력받은 6개의 숫자 서버로 전송

		int correct[1];//서버로부터 받은 숫자의 일치 갯수 저장되어 있는 배열
		recv(c_Socket, (char*)correct, sizeof(correct), 0);//서버로부터 값 전달받음
		
		int count = ntohl(correct[0]);
		printf("%d개의 번호가 일치합니다.\n\n", count);

		if (count== 6) {
			printf("로또 번호 맞추기 게임을 종료합니다.\n\n");
			break;
		}
	}
	closesocket(c_Socket);
}