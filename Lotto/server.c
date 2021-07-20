#pragma comment(lib,"wsock32.lib")

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <stdlib.h>
#include <time.h>

#define PORT 5500

unsigned __stdcall  LottoGame(void* arg);

int Lottonumber[6];//���� ���� ���� ����
unsigned threadID;//������ id ���� ����

int main() {

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	SOCKET s_Socket = socket(AF_INET, SOCK_STREAM, 0);//���� ���� ����
	printf("���� ������ �����߽��ϴ�.\n");

	SOCKADDR_IN serverAddress;//���� �ּ� �����ϴ� ����ü ����
	ZeroMemory(&serverAddress, 0, sizeof(serverAddress));//����ü �ʱ�ȭ
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);

	bind(s_Socket, (SOCKADDR*)& serverAddress, sizeof(serverAddress));
	listen(s_Socket, 5);
	printf("Ŭ���̾�Ʈ ���� ��� �����Դϴ�.\n\n");
	
	//���� ����
	srand((unsigned)time(NULL));	
	for (int i = 0; i < 6; i++)
	{
		Lottonumber[i] = ((rand() % 45) + 1);
		for (int j=i-1; j>=0; j--)
		{
			if (Lottonumber[i] == Lottonumber[j])
			{
				i--;
				break;
			}
		}
	}
	printf("������ �ζ� ��ȣ : %d %d %d %d %d %d\n\n", Lottonumber[0], Lottonumber[1], Lottonumber[2], Lottonumber[3], Lottonumber[4], Lottonumber[5]);

	while (1) {
		int c_Socket = accept(s_Socket, NULL, NULL);
		printf("Ŭ���̾�Ʈ�� �����߽��ϴ�.\n");
		printf("�ζ� ��ȣ ���߱� ������ �����մϴ�.\n\n");

		unsigned long hThread = _beginthreadex(NULL, 0, LottoGame, (void*)c_Socket, 0, &threadID);
	}
	return 0;
}

unsigned __stdcall  LottoGame(void* arg) {

	int c_Socket = (int)arg;
	while (1) {
		int clnt_num[6];//�Է¹��� 6���� ���ڸ� ������ �迭
		int clntcorr[1];//������ ��ġ ���� ������ �迭(Ŭ���̾�Ʈ ���� ����)
	 
		recv(c_Socket, (char*)clnt_num, sizeof(clnt_num), 0);//Ŭ���̾�Ʈ�κ��� ������ ����
		
		for (int num = 0; num < 6; num++)
			clnt_num[num] = ntohl(clnt_num[num]);
	
		printf("Ŭ���̾�Ʈ�� �ζ� ��ȣ : %d %d %d %d %d %d\n", clnt_num[0], clnt_num[1], clnt_num[2], clnt_num[3], clnt_num[4], clnt_num[5]);

		int count = 0;//�ζ� ��ȣ ��ġ�ϴ� ���� ���� ����

		for (int i = 0; i < 6; i++) {
			int serv_num = Lottonumber[i];

			for (int j = 0; j < 6; j++) {
				int clnt_num_new = clnt_num[j];

				if (serv_num == clnt_num_new) {
					count++;
				}
			}
		}
		printf("%d���� ��ġ�մϴ�.\n\n", count);
	
		clntcorr[0] = htonl(count);
	
		send(c_Socket, (char*)clntcorr, sizeof(clntcorr), 0);//Ŭ���̾�Ʈ�� ������ ����

		if (count==6) {//6���� ��ȣ�� ��ġ�� �� ����
			printf("���� Ŭ���̾�Ʈ�� ������ �����մϴ�.\n");
			break;
		}
	}
	closesocket(c_Socket);//Ŭ���̾�Ʈ ���� ����
	return 0;
}