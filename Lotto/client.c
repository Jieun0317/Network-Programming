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
	
	SOCKET c_Socket = socket(AF_INET, SOCK_STREAM, 0);//Ŭ���̾�Ʈ ���� ����

	if (c_Socket == INVALID_SOCKET) {
		printf("error\n");
		return 0;
	}
	printf("Ŭ���̾�Ʈ ���� ���� �Ϸ��߽��ϴ�.\n\n");

	SOCKADDR_IN serverAddress;//�ּ� ���� ����ü
	ZeroMemory(&serverAddress, 0, sizeof(serverAddress));//����ü �ʱ�ȭ

	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);

	connect(c_Socket, (SOCKADDR*)& serverAddress, sizeof(serverAddress));

	printf("�ζ� ��ȣ ���߱⸦ �����մϴ�.\n\n");

	while (1) {
		int number[6];

		printf("6���� �ζ� ��ȣ�� �Է��� �ּ���.(���� : 1 ~ 45) : ");
		scanf_s("%d %d %d %d %d %d", &number[0], &number[1], &number[2], &number[3], &number[4], &number[5]);
	
		for (int i = 0; i < 6; i++)
			number[i] = htonl(number[i]);
		
		send(c_Socket, (char*)number, sizeof(number), 0);//�Է¹��� 6���� ���� ������ ����

		int correct[1];//�����κ��� ���� ������ ��ġ ���� ����Ǿ� �ִ� �迭
		recv(c_Socket, (char*)correct, sizeof(correct), 0);//�����κ��� �� ���޹���
		
		int count = ntohl(correct[0]);
		printf("%d���� ��ȣ�� ��ġ�մϴ�.\n\n", count);

		if (count== 6) {
			printf("�ζ� ��ȣ ���߱� ������ �����մϴ�.\n\n");
			break;
		}
	}
	closesocket(c_Socket);
}